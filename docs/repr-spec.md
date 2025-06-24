# BigNumber Structure

```
BigNumber {
    sign: boolean           // number sign (false = "+", true = "-")
    mantissa: chunk[]       // dynamic array of chunks (reverse order)
    shift: integer          // chunk position shift
    type: NumberType        // DEFAULT | ZERO | INF | NOT_A_NUMBER
}
```
---

# System Constants
| Constant     | Description                                    |
| ------------ | ---------------------------------------------- |
| `BASE`       | Number of decimal digits in one chunk          |
| `MAX_CHUNKS` | Maximum number of chunks in mantissa           |
| `MAX_SHIFT`  | Maximum absolute value of shift                |

---

# Representation Rules

## Mantissa
- **Order**: least significant digits at the beginning of array
- **Chunks**: integers from 0 to (10^BASE - 1)
- **Normalization**: no zero chunks at both ends
- **Exception**: empty mantissa for ZERO, INF, NOT_A_NUMBER types

## Number Types
- **DEFAULT** - regular number (mantissa not empty)
- **ZERO** - zero (mantissa may be empty)
- **INF** - infinity (mantissa ignored)
- **NOT_A_NUMBER** - not a number (mantissa ignored)

## Sign
- **false** = positive number
- **true** = negative number
- Applies to all number types

---

# Implementation Requirements
## Validity
✓ Type from allowed values
✓ Mantissa normalized
✓ Chunks in range [0, 10^BASE)
✓ Shift within ±MAX_SHIFT bounds

## Normalization
1. Remove zero chunks from both ends of mantissa
2. If mantissa becomes empty → set type to ZERO
3. Ensure uniqueness of representation

## BASE Selection
- **32-bit chunks**: BASE ≤ 10^9
- **64-bit chunks**: BASE ≤ 10^18
- Consider overflow in arithmetic operations
