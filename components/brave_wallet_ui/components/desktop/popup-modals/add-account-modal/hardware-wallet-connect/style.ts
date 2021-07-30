import styled from 'styled-components'

import LedgerLogo from '../../../../../assets/svg-icons/ledger-logo.svg'
import TrezorLogo from '../../../../../assets/svg-icons/trezor-logo.svg'

interface StyleProps {
  isSelected: boolean
}

export const HardwareTitle = styled.span`
  font-family: Poppins;
  font-size: 14px;
  line-height: 20px;
  font-weight: 600;
  letter-spacing: 0.01em;
  color: ${(p) => p.theme.color.text02};
  margin-bottom: 25px;
`

export const HardwareButtonRow = styled.div`
  display: flex;
  flex-direction: row;
  align-items: center;
  justify-content: space-between;
  width: 260px;
  margin-bottom: 35px;
`

export const HardwareButton = styled.button<StyleProps>`
  display: flex;
  align-items: center;
  justify-content: center;
  flex-direction: row;
  cursor: pointer;
  outline: none;
  background: none;
  border: ${(p) => p.isSelected ? `2px solid ${p.theme.color.infoBorder}` : `1px solid ${p.theme.color.disabled}`};
  background-color: ${(p) => p.isSelected ? p.theme.color.infoBackground : p.theme.color.background02};
  border-radius: 10px;
  width: 125px;
  height: 55px;
`

export const LedgerIcon = styled.div`
  width: 93px;
  height: 25px;
  background-color: ${(p) => p.theme.color.interactive07};
  -webkit-mask-image: url(${LedgerLogo});
  mask-image: url(${LedgerLogo});
`

export const TrezorIcon = styled.div`
  width: 105px;
  height: 33px;
  background-color: ${(p) => p.theme.color.interactive07};
  -webkit-mask-image: url(${TrezorLogo});
  mask-image: url(${TrezorLogo});
`

export const HardwareInfoRow = styled.div`
  display: flex;
  flex-direction: row;
  align-items: flex-start;
  justify-content: flex-start;
  margin-bottom: 35px;
`

export const HardwareInfoColumn = styled.div`
  display: flex;
  flex-direction: column;
  align-items: flex-start;
  justify-content: flex-start;
  margin-left: 10px;
`

export const ConnectingButton = styled.button`
  display: flex;
  flex-direction: column;
  justify-content: center;
  align-items: center;
  padding: 10px 22px;

  border: 1px solid ${(p) => p.theme.color.interactive08};
  box-sizing: border-box;
  border-radius: 48px;

  // Added manually
  background-color: ${(p) => p.theme.palette.white};
`

export const ConnectingButtonText = styled.span`
  font-family: Poppins;
  font-style: normal;
  font-weight: 600;
  font-size: 13px;
  line-height: 20px;
  text-align: center;

  /* Light Theme/Brand/interactive07 */
  color: ${(p) => p.theme.color.interactive07};

  /* Inside Auto Layout */
  flex: none;
  order: 1;
  flex-grow: 0;
  margin: 0px 8px;
`
