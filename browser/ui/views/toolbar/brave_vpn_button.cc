/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/ui/views/toolbar/brave_vpn_button.h"

#include <memory>
#include <utility>

#include "brave/app/vector_icons/vector_icons.h"
#include "brave/grit/brave_generated_resources.h"
#include "chrome/browser/themes/theme_properties.h"
#include "chrome/browser/ui/layout_constants.h"
#include "chrome/browser/ui/views/toolbar/toolbar_ink_drop_util.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/base/metadata/metadata_impl_macros.h"
#include "ui/gfx/geometry/size.h"
#include "ui/gfx/paint_vector_icon.h"
#include "ui/gfx/skia_util.h"
#include "ui/views/background.h"
#include "ui/views/border.h"
#include "ui/views/controls/highlight_path_generator.h"

namespace {

constexpr int kHighlightRadius = 47;

class BraveVPNButtonHighlightPathGenerator
    : public views::HighlightPathGenerator {
 public:
  BraveVPNButtonHighlightPathGenerator() = default;
  BraveVPNButtonHighlightPathGenerator(
      const BraveVPNButtonHighlightPathGenerator&) = delete;
  BraveVPNButtonHighlightPathGenerator& operator=(
      const BraveVPNButtonHighlightPathGenerator&) = delete;

  // views::HighlightPathGenerator overrides:
  SkPath GetHighlightPath(const views::View* view) override {
    gfx::Rect rect(view->size());
    rect.Inset(GetToolbarInkDropInsets(view));
    SkPath path;
    path.addRoundRect(gfx::RectToSkRect(rect), kHighlightRadius,
                      kHighlightRadius);
    return path;
  }
};

}  // namespace

BraveVPNButton::BraveVPNButton()
    : ToolbarButton(base::BindRepeating(&BraveVPNButton::OnButtonPressed,
                                        base::Unretained(this))) {
  // Replace ToolbarButton's highlight path generator.
  views::HighlightPathGenerator::Install(
      this, std::make_unique<BraveVPNButtonHighlightPathGenerator>());

  gfx::FontList font_list = views::Label::GetDefaultFontList();
  constexpr int kFontSize = 12;
  label()->SetFontList(
      font_list.DeriveWithSizeDelta(kFontSize - font_list.GetFontSize()));

  // Set image positions first. then label.
  SetHorizontalAlignment(gfx::ALIGN_LEFT);

  UpdateButtonState();
}

BraveVPNButton::~BraveVPNButton() = default;

void BraveVPNButton::UpdateColorsAndInsets() {
  if (const auto* tp = GetThemeProvider()) {
    const gfx::Insets paint_insets =
        gfx::Insets((height() - GetLayoutConstant(LOCATION_BAR_HEIGHT)) / 2);
    SetBackground(views::CreateBackgroundFromPainter(
        views::Painter::CreateSolidRoundRectPainter(
            tp->GetColor(ThemeProperties::COLOR_TOOLBAR), kHighlightRadius,
            paint_insets)));
  }

  // TODO(simonhong): consider themed border color.
  constexpr SkColor kBorderColor = SkColorSetRGB(0xE1, 0xE1, 0xE1);
  std::unique_ptr<views::Border> border = views::CreateRoundedRectBorder(
      1, kHighlightRadius, gfx::Insets(), kBorderColor);

  constexpr gfx::Insets kTargetInsets{4, 6};
  const gfx::Insets extra_insets = kTargetInsets - border->GetInsets();
  SetBorder(views::CreatePaddedBorder(std::move(border), extra_insets));

  constexpr int kBraveAvatarImageLabelSpacing = 4;
  SetImageLabelSpacing(kBraveAvatarImageLabelSpacing);
  return;
}

void BraveVPNButton::UpdateButtonState() {
  label()->SetText(l10n_util::GetStringUTF16(
      IsConntected() ? IDS_BRAVE_VPN_TOOLBAR_BUTTON_CONNECTED_TEXT
                     : IDS_BRAVE_VPN_TOOLBAR_BUTTON_DISCONNECTED_TEXT));

  constexpr SkColor kColorConnected = SkColorSetRGB(0x51, 0xCF, 0x66);
  constexpr SkColor kColorDisconnected = SkColorSetRGB(0xAE, 0xB1, 0xC2);
  SetImage(views::Button::STATE_NORMAL,
           gfx::CreateVectorIcon(kVpnIndicatorIcon, IsConntected()
                                                        ? kColorConnected
                                                        : kColorDisconnected));
}

bool BraveVPNButton::IsConntected() {
  // TODO(simonhong): Get connection status when service is ready to use.
  NOTIMPLEMENTED();
  return true;
}

void BraveVPNButton::OnButtonPressed(const ui::Event& event) {
  ShowBraveVPNPanel();
}

void BraveVPNButton::ShowBraveVPNPanel() {
  NOTIMPLEMENTED();
}

BEGIN_METADATA(BraveVPNButton, LabelButton)
END_METADATA
