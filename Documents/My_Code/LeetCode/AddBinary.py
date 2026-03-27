class Solution:
    def addBinary(self, a: str, b: str) -> str:
        binary_string = a
# The '2' tells Python the input is in base-2 (binary)
        decimal_valuea = int(binary_string, 2)
        binary_string = b
# The '2' tells Python the input is in base-2 (binary)
        decimal_valueb = int(binary_string, 2)
        add= (decimal_valuea + decimal_valueb)

        decimal_num = add
# Returns binary string without prefix
        binary_string = format(decimal_num, 'b') 
        return binary_string
