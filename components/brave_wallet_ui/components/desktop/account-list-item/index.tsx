import * as React from 'react'
import { reduceAddress } from '../../../utils/reduce-address'
import { copyToClipboard } from '../../../utils/copy-to-clipboard'
import { create } from 'ethereum-blockies'
import { Tooltip } from '../../shared'
import { WalletAccountType } from '../../../constants/types'
import locale from '../../../constants/locale'

// Styled Components
import {
  StyledWrapper,
  AccountName,
  AccountAddress,
  AccountAndAddress,
  NameAndIcon,
  AccountCircle,
  RightSide,
  HardwareIcon,
  AccountNameRow
} from './style'

export interface Props {
  onDelete?: () => void
  onClick: (account: WalletAccountType) => void
  account: WalletAccountType
  isHardwareWallet: boolean
}

function AccountListItem (props: Props) {
  const {
    account,
    isHardwareWallet,
    onClick
  } = props

  const onCopyToClipboard = async () => {
    await copyToClipboard(account.address)
  }

  const onSelectAccount = () => {
    onClick(account)
  }

  const orb = React.useMemo(() => {
    return create({ seed: account.address, size: 8, scale: 16 }).toDataURL()
  }, [account.address])

  return (
    <StyledWrapper>
      <NameAndIcon>
        <AccountCircle orb={orb} />
        <AccountAndAddress>
          <AccountNameRow>
            {isHardwareWallet && <HardwareIcon />}
            <AccountName onClick={onSelectAccount}>{account.name}</AccountName>
          </AccountNameRow>
          <Tooltip text={locale.toolTipCopyToClipboard}>
            <AccountAddress onClick={onCopyToClipboard}>{reduceAddress(account.address)}</AccountAddress>
          </Tooltip>
        </AccountAndAddress>
      </NameAndIcon>
      <RightSide>
        {/* <DeleteButton onClick={onDelete}>
          <DeleteIcon />
        </DeleteButton> */}
      </RightSide>
    </StyledWrapper>
  )
}

export default AccountListItem