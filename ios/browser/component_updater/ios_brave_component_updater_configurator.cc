/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/ios/browser/component_updater/ios_brave_component_updater_configurator.h"

#include <stdint.h>

#include <memory>
#include <string>
#include <vector>

#include "base/command_line.h"
#include "base/strings/string_number_conversions.h"
#include "base/strings/sys_string_conversions.h"
#include "base/version.h"
#include "brave/common/brave_switches.h"
#include "build/build_config.h"
#include "components/component_updater/component_updater_command_line_config_policy.h"
#include "components/prefs/pref_registry_simple.h"
#include "components/prefs/pref_service.h"
#include "components/services/patch/in_process_file_patcher.h"
#include "components/services/unzip/in_process_unzipper.h"
#include "components/update_client/activity_data_service.h"
#include "components/update_client/crx_downloader_factory.h"
#include "components/update_client/net/network_chromium.h"
#include "components/update_client/patch/patch_impl.h"
#include "components/update_client/protocol_handler.h"
#include "components/update_client/unzip/unzip_impl.h"
#include "components/update_client/unzipper.h"
#include "components/update_client/update_query_params.h"
#include "ios/chrome/browser/application_context.h"
#include "ios/web/public/thread/web_thread.h"
#include "services/network/public/cpp/shared_url_loader_factory.h"

namespace component_updater {

// Allows the component updater to use non-encrypted communication with the
// update backend. The security of the update checks is enforced using
// a custom message signing protocol and it does not depend on using HTTPS.
IOSBraveConfigurator::IOSBraveConfigurator(const base::CommandLine* cmdline)
    : configurator_impl_(ComponentUpdaterCommandLineConfigPolicy(cmdline),
                         false) {}

IOSBraveConfigurator::~IOSBraveConfigurator() {}

double IOSBraveConfigurator::InitialDelay() const {
  return configurator_impl_.InitialDelay();
}

int IOSBraveConfigurator::NextCheckDelay() const {
  auto* command = base::CommandLine::ForCurrentProcess();
  if (command->HasSwitch(switches::kComponentUpdateIntervalInSec)) {
    int interval = 0;
    if (base::StringToInt(command->GetSwitchValueASCII(
                              switches::kComponentUpdateIntervalInSec),
                          &interval)) {
      DCHECK_GE(interval, 1);
      return interval;
    }
  }
  return configurator_impl_.NextCheckDelay();
}

int IOSBraveConfigurator::OnDemandDelay() const {
  return configurator_impl_.OnDemandDelay();
}

int IOSBraveConfigurator::UpdateDelay() const {
  return configurator_impl_.UpdateDelay();
}

std::vector<GURL> IOSBraveConfigurator::UpdateUrl() const {
  return configurator_impl_.UpdateUrl();
}

std::vector<GURL> IOSBraveConfigurator::PingUrl() const {
  return configurator_impl_.PingUrl();
}

std::string IOSBraveConfigurator::GetProdId() const {
  return std::string();
}

base::Version IOSBraveConfigurator::GetBrowserVersion() const {
  return configurator_impl_.GetBrowserVersion();
}

std::string IOSBraveConfigurator::GetChannel() const {
  return std::string("stable");
}

std::string IOSBraveConfigurator::GetBrand() const {
  return std::string();
}

std::string IOSBraveConfigurator::GetLang() const {
  return std::string();
}

std::string IOSBraveConfigurator::GetOSLongName() const {
  return configurator_impl_.GetOSLongName();
}

base::flat_map<std::string, std::string>
IOSBraveConfigurator::ExtraRequestParams() const {
  return configurator_impl_.ExtraRequestParams();
}

std::string IOSBraveConfigurator::GetDownloadPreference() const {
  return std::string();
}

scoped_refptr<update_client::NetworkFetcherFactory>
IOSBraveConfigurator::GetNetworkFetcherFactory() {
  if (!network_fetcher_factory_) {
    network_fetcher_factory_ =
        base::MakeRefCounted<update_client::NetworkFetcherChromiumFactory>(
            GetApplicationContext()->GetSharedURLLoaderFactory(),
            // Never send cookies for component update downloads.
            base::BindRepeating([](const GURL& url) { return false; }));
  }
  return network_fetcher_factory_;
}

scoped_refptr<update_client::CrxDownloaderFactory>
IOSBraveConfigurator::GetCrxDownloaderFactory() {
  if (!crx_downloader_factory_) {
    crx_downloader_factory_ =
        update_client::MakeCrxDownloaderFactory(GetNetworkFetcherFactory());
  }
  return crx_downloader_factory_;
}

scoped_refptr<update_client::UnzipperFactory>
IOSBraveConfigurator::GetUnzipperFactory() {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  if (!unzip_factory_) {
    unzip_factory_ = base::MakeRefCounted<update_client::UnzipChromiumFactory>(
        base::BindRepeating(&unzip::LaunchInProcessUnzipper));
  }
  return unzip_factory_;
}

scoped_refptr<update_client::PatcherFactory>
IOSBraveConfigurator::GetPatcherFactory() {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  if (!patch_factory_) {
    patch_factory_ = base::MakeRefCounted<update_client::PatchChromiumFactory>(
        base::BindRepeating(&patch::LaunchInProcessFilePatcher));
  }
  return patch_factory_;
}

bool IOSBraveConfigurator::EnabledDeltas() const {
  return configurator_impl_.EnabledDeltas();
}

bool IOSBraveConfigurator::EnabledComponentUpdates() const {
  return configurator_impl_.EnabledComponentUpdates();
}

bool IOSBraveConfigurator::EnabledBackgroundDownloader() const {
  return configurator_impl_.EnabledBackgroundDownloader();
}

bool IOSBraveConfigurator::EnabledCupSigning() const {
  return false;
}

PrefService* IOSBraveConfigurator::GetPrefService() const {
  return GetApplicationContext()->GetLocalState();
}

update_client::ActivityDataService*
IOSBraveConfigurator::GetActivityDataService() const {
  return nullptr;
}

bool IOSBraveConfigurator::IsPerUserInstall() const {
  return false;
}

std::unique_ptr<update_client::ProtocolHandlerFactory>
IOSBraveConfigurator::GetProtocolHandlerFactory() const {
  return configurator_impl_.GetProtocolHandlerFactory();
}

}  // namespace component_updater
