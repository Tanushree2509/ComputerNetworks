# Hamming (7,4) Code

# Input 4 data bits
d1 = int(input("Enter D1: "))
d2 = int(input("Enter D2: "))
d3 = int(input("Enter D3: "))
d4 = int(input("Enter D4: "))

# Calculate parity bits (Even parity)
p1 = d1 ^ d2 ^ d4
p2 = d1 ^ d3 ^ d4
p4 = d2 ^ d3 ^ d4

# Generate Hamming code
hamming = [p1, p2, d1, p4, d2, d3, d4]

print("\nGenerated Hamming Code:")
print(*hamming)

# Receive 7-bit code
received = list(map(int, input(
    "\nEnter received 7-bit code (space-separated): "
).split()))

# Check parity
c1 = received[0] ^ received[2] ^ received[4] ^ received[6]
c2 = received[1] ^ received[2] ^ received[5] ^ received[6]
c4 = received[3] ^ received[4] ^ received[5] ^ received[6]

# Find error position
error_pos = c1 + (c2 * 2) + (c4 * 4)

if error_pos == 0:
    print("\nNo error detected.")
else:
    print(f"\nError detected at bit position {error_pos}")

    # Correct the error
    received[error_pos - 1] ^= 1

    print("Corrected Hamming Code:")
    print(*received)