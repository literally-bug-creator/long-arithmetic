# Input Structure

```
BigNumberInput {
    digits: uint8_t[]       // dynamic array of significant digits
    exponent: int32_t       // exponent value
    is_negative: bool       // number sign (false = "+", true = "-")
    error: Error           // error state (no exceptions mechanism)
}
```

---

# System Constants

| Constant     | Description                             |
| ------------ | --------------------------------------- |
| `MAX_DIGITS` | Maximum number of digits in input array |
| `MAX_EXP`    | Maximum absolute value of exponent      |

---

# Input Rules

## Digits Array

- **Type**: dynamic array of `uint8_t`
- **Content**: significant digits of the number
- **Leading zeros**: must be strictly avoided as they are retained during chunk formation and can cause computational errors
- **Trailing zeros**: allowed, removed during chunk formation and accounted for in exponent normalization
- **Size limit**: array length ≤ MAX_DIGITS before normalization

## Exponent

- **Type**: `int32_t`
- **Range**: |exponent| ≤ MAX_EXP
- **Modification**: adjusted during digit normalization

## Sign

- **Type**: `bool`
- **false** = positive number
- **true** = negative number

## Error Handling

- **Mechanism**: error propagation (no exceptions)
- **Storage**: each number contains Error state
- **Propagation**: errors pass from number to number through operations

---

# Normalization Process

## Validation Order

1. Check digits array length ≤ MAX_DIGITS
2. Check |exponent| ≤ MAX_EXP
3. Perform normalization
4. Recheck |exponent| ≤ MAX_EXP after normalization

---

# Implementation Requirements

## Input Validation

✓ Digits array size within limits before normalization
✓ Exponent within allowed bounds
✓ Post-normalization exponent validation

## Error Propagation

✓ No exception throwing mechanism
✓ Error state preserved in number structure
✓ Errors propagate through all operations
✓ Clear error reporting and handling

## Normalization

✓ Exponent correctly adjusted, including adjustments for trailing zeros
✓ Leading zeros must be strictly avoided to prevent computational errors during chunk formation
✓ Resulting number in canonical form
