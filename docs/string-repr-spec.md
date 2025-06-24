# BigNumber Output Specification

# General Format

```
[sign][significant_part][exponent]
```

---

# String Formation Rules

## Sign

- **Negative number** (`is_negative = true`) → "-" prefix
- **Positive number** (`is_negative = false`) → no prefix
- Applies to all number types, including special values

## Special Values

| Type           | String Representation |
| -------------- | --------------------- |
| `ZERO`         | `[sign]0`             |
| `INF`          | `[sign]inf`           |
| `NOT_A_NUMBER` | `[sign]NaN`           |

## Regular Numbers (`DEFAULT`)

### Significant Part

- **Source**: mantissa with shift applied
- **Normalization**: no leading zeros
- **Normalization**: no trailing zeros
- **Format**: sequence of decimal digits

### Exponent

- **Condition**: added only if value ≠ 0
- **Format**: "e" + decimal number
- **Value**: decimal point position relative to significant digits

---

# Implementation Requirements

## Precision

✓ All significant digits must be preserved
✓ Normalization must not lose precision
✓ Exponent calculated correctly

## Readability

✓ Minimum string length while preserving precision
✓ Standard scientific format for large numbers
✓ Avoid redundant zeros
