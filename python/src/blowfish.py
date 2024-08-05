import random


class Blowfish:
    BLOCK_SIZE = 8  # 64 bits
    KEY_SIZE = 56  # 448 bits
    ROUNDS = 16

    def __init__(self, key):
        self.key = key
        self.P_ARRAY = [0] * 18
        self.S_BOXES = [[0] * 256 for _ in range(4)]
        self.initialize_p_array_and_s_boxes()
        self.key_expansion()

    def initialize_p_array_and_s_boxes(self):
        # Initialize P-Array and S-Boxes with random values
        random.seed(0)  # For reproducibility
        for i in range(18):
            self.P_ARRAY[i] = random.getrandbits(32)
        for i in range(4):
            for j in range(256):
                self.S_BOXES[i][j] = random.getrandbits(32)

    def key_expansion(self):
        key_bytes = self.key.to_bytes((self.key.bit_length() + 7) // 8, "big")
        key_index = 0
        for i in range(18):
            self.P_ARRAY[i] ^= int.from_bytes(
                key_bytes[key_index : key_index + 4], "big"
            )
            key_index = (key_index + 4) % len(key_bytes)

    def encrypt_block(self, block):
        left, right = block[:4], block[4:]
        left = int.from_bytes(left, "big")
        right = int.from_bytes(right, "big")

        for i in range(16):
            left = left ^ self.P_ARRAY[i]
            right = self.feistel_function(left) ^ right
            left, right = right % 0xFFFFFFFF, left % 0xFFFFFFFF

        left, right = right, left
        right = right ^ self.P_ARRAY[16]
        left = left ^ self.P_ARRAY[17]

        return left.to_bytes(4, "big") + right.to_bytes(4, "big")

    def feistel_function(self, half_block):
        a = (half_block >> 24) & 0xFF
        b = (half_block >> 16) & 0xFF
        c = (half_block >> 8) & 0xFF
        d = half_block & 0xFF % 0xFFFFFFFF

        return (
            (self.S_BOXES[0][a] + self.S_BOXES[1][b]) ^ self.S_BOXES[2][c]
        ) % 0xFFFFFFFF + self.S_BOXES[3][d] % 0xFFFFFFFF

    def decrypt_block(self, block):
        """
        Decrypt a single 64-bit block using the Blowfish algorithm.
        """
        left, right = block[:4], block[4:]
        left = int.from_bytes(left, "big")
        right = int.from_bytes(right, "big")

        for i in range(17, 1, -1):
            left = left ^ self.P_ARRAY[i]
            right = self.feistel_function(left) ^ right
            left, right = right % 0xFFFFFFFF, left % 0xFFFFFFFF

        left, right = right, left
        right = right ^ self.P_ARRAY[1]
        left = left ^ self.P_ARRAY[0]

        return left.to_bytes(4, "big") + right.to_bytes(4, "big")


if __name__ == "__main__":
    key = 0x1234567890ABCDEF  # Example key
    blowfish = Blowfish(key)

    plaintext = b"This is a long string to be encrypted using Blowfish algorithm."
    encrypted = b""
    for i in range(0, len(plaintext), Blowfish.BLOCK_SIZE):
        block = plaintext[i : i + Blowfish.BLOCK_SIZE]
        if len(block) < Blowfish.BLOCK_SIZE:
            block += b"\0" * (Blowfish.BLOCK_SIZE - len(block))
        encrypted_block = blowfish.encrypt_block(block)
        encrypted += encrypted_block

    print(f"Plaintext: {plaintext}")
    print(f"Encrypted: {encrypted}")

    # Decryption
    decrypted = b""
    for i in range(0, len(encrypted), Blowfish.BLOCK_SIZE):
        block = encrypted[i : i + Blowfish.BLOCK_SIZE]
        decrypted_block = blowfish.decrypt_block(block)
        decrypted += decrypted_block

    # Remove padding
    decrypted = decrypted.rstrip(b"\0")

    print(f"Decrypted: {decrypted}")


