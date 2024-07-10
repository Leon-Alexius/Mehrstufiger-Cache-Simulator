## `getopt_long()`

```c
int getopt_long(int argc, char* const argv[], const char* optstring, const struct option* longopts, int* longindex);
```

The parameters:
- `argc`: The number of command-line args.
- `argv`: The array of command-line args.
- `optstring`: A string of letters of valid short options. Options with required argument is followed by a colon.
- `longopts`: An array of `struct option` elements, one for each long option. 

    **Note**: Termination is an element containing all zeros.

- `longindex`: A pointer for `longopts`. If not NULL `getopt_long()` will store the index of the long option variant here.

Notes:
- Returns `val` or -1 if there are no more options. 
- If option has an argument, `getopt_long()` makes `optarg` point to it. 

## `struct option`
The `struct option` defines the long options.

```c
struct option {
    const char* name;       // The name of the long option.
    int has_arg;            // no_argument, required_argument, optional_argument.
    int* flag;              // If not NULL, getopt_long() sets flag to val when the option is found, and returns 0.
    int val;                // The value to return, or to load into the variable pointed to by flag.
};
```

## `strtoul()`
Simply string into an unsigned long integer

```c
unsigned long int strtoul(const char* nptr, char** endptr, int base);
```

The parameters
- `nptr`: The string.
- `endptr`: If `endptr` is not NULL, `strtoul()` stores the address of the first invalid character in `*endptr`. If there are no digits at all, `strtoul()` stores the original value of `nptr` in `*endptr`.
- `base`: The base of the unsigned long integer in the string pointed to by `nptr`. Between 2 and 36 inclusive, or 0.

Note: 
- returns `ULONG_MAX` if overflow. 
- Sets `errno` to `ERANGE` if overflow.
