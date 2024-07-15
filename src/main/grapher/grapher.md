# About `size_t`

`size_t` is a type defined in the C and C++ standard libraries. It is used for representing the sizes of objects in memory and is an unsigned integral type. Due to being specifically designed to represent sizes, it is guaranteed to be large enough to contain the size of the largest possible object on the target platform. 

Notes:
- It is not always the same as `unsigned int`, as its exact type can vary depending on the system architecture and compiler.
- The precise definition of `size_t` can vary depending on the platform and compiler being used.

More about `size_t`:

1. **Purpose**: `size_t` is intended to be the type that can represent the size of any object in memory. It is commonly used for array indexing and loop counting where the size or count cannot be negative.

2. **Definition**: It is defined in the header files `<stddef.h>` (for C) and `<cstddef>` (for C++).

3. **Implementation**: The actual underlying type of `size_t` is implementation-defined. It is typically defined as an alias for one of the unsigned integer types, such as `unsigned int`, `unsigned long`, or `unsigned long long`. The choice depends on the architecture and compiler. For instance:
   - On a 32-bit system, `size_t` is often defined as `unsigned int` or `unsigned long`, both being 32 bits in size.
   - On a 64-bit system, `size_t` is often defined as `unsigned long` or `unsigned long long`, both being 64 bits in size.

4. **Example Usage**:
   ```c
   #include <stdio.h>
   #include <stddef.h>

   int main() {
       size_t size = sizeof(int);  // size_t used to hold the size of an int
       printf("Size of int: %zu bytes\n", size);
       return 0;
   }
   ```

5. **Compatibility**: `size_t` is guaranteed to be able to represent the size of any object in memory, it avoids the pitfalls of using an `unsigned int`, which might be too small on some platforms. (make code more robust + portable)

6. **Printing `size_t`**: When printing a `size_t` value using `printf`, we use the `"%zu"` format specifier to ensure proper handling across different platforms.

# Format Specifiers For `printf()`

| **Specifier** | **Description**                                                                 | **Example**              |
|----------------|---------------------------------------------------------------------------------|--------------------------|
| `%d` or `%i`   | Signed decimal integer                                                          | `123`                    |
| `%u`           | Unsigned decimal integer                                                        | `123`                    |
| `%o`           | Unsigned octal integer                                                          | `173`                    |
| `%x`           | Unsigned hexadecimal integer (lowercase)                                        | `7b`                     |
| `%X`           | Unsigned hexadecimal integer (uppercase)                                        | `7B`                     |
| `%f`           | Decimal floating point                                                          | `123.456`                |
| `%e`           | Scientific notation (lowercase)                                                 | `1.234560e+02`           |
| `%E`           | Scientific notation (uppercase)                                                 | `1.234560E+02`           |
| `%g`           | Use `%e` or `%f` whichever is shorter (lowercase)                               | `123.456` or `1.23456e2` |
| `%G`           | Use `%E` or `%f` whichever is shorter (uppercase)                               | `123.456` or `1.23456E2` |
| `%a`           | Hexadecimal floating point (lowercase)                                          | `0x1.91eb86p+1`          |
| `%A`           | Hexadecimal floating point (uppercase)                                          | `0X1.91EB86P+1`          |
| `%c`           | Character                                                                       | `A`                      |
| `%s`           | String of characters                                                            | `"Hello, world!"`        |
| `%p`           | Pointer address                                                                 | `0x7ffeed5e94b8`         |
| `%n`           | Number of characters printed so far (writes to an integer variable)             |                          |
| `%zu`          | Size_t (unsigned decimal integer)                                               | `123`                    |
| `%zd`          | Size_t (signed decimal integer)                                                 | `-123`                   |
| `%%`           | Percent sign                                                                    | `%`                      |

### Length Modifiers
Length modifiers can be used with the format specifiers to specify the size of the data type:

| **Length Modifier** | **Description**                                                   | **Example**          |
|---------------------|-------------------------------------------------------------------|-----------------------|
| `h`                 | Short integer (applies to `%d`, `%i`, `%u`, `%o`, `%x`, `%X`)     | `%hd` (short int)     |
| `l`                 | Long integer (applies to `%d`, `%i`, `%u`, `%o`, `%x`, `%X`)      | `%ld` (long int)      |
| `ll`                | Long long integer (applies to `%d`, `%i`, `%u`, `%o`, `%x`, `%X`) | `%lld` (long long int)|
| `L`                 | Long double (applies to `%f`, `%e`, `%E`, `%g`, `%G`, `%a`, `%A`) | `%Lf` (long double)   |

### Notes
- The `%zu` and `%zd` specifiers are specifically used for `size_t` values, (for correct handling across different platforms + avoid potential issues with type size mismatches).
