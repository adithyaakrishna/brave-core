import { formatInputValue } from './format-balances'

describe('formatInputValue', () => {
  it.each([
    ['0x0', 18, '0'], // 0 ETH -> 0 ETH
    ['0xde0b6b3a7640000', 18, '1'], // 1 ETH -> 1ETH
    ['0xde0b6b3a7640001', 18, '1'], // 1.000000000000000001 ETH -> 1 ETH
    ['0xde444324c2a8080', 18, '1.001'], // 1.001000000000000001 ETH -> 1.001 ETH
    ['0x1', 18, '0.000000000000000001'] // 0.000000000000000001 ETH
  ])('should format %s with %s decimals to %s', (amount: string, decimals: number, expected) => {
    expect(formatInputValue(amount, decimals)).toBe(expected)
  })

  it('should not round if parameter is overridden', () => {
    expect(formatInputValue('0xde0b6b3a7640001', 18)).toBe('1')
    expect(formatInputValue('0xde0b6b3a7640001', 18, false)).toBe('1.000000000000000001')
  })
})
