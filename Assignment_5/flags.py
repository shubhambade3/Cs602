class EFlags:
    def __init__(self):
       
        self.value = 0x00000002 

    def update_arithmetic_flags(self, result, operand1, operand2, is_subtraction=False, bit_width=32):
        """
        Updates CF, PF, AF, ZF, SF, and OF based on the result of an operation.
        """
        max_val = (1 << bit_width) - 1
        msb_mask = 1 << (bit_width - 1)
        
        # 1. Zero Flag (ZF)
        masked_result = result & max_val
        if masked_result == 0:
            self.value |= (1 << 6)   
        else:
            self.value &= ~(1 << 6)  

        # 2. Sign Flag (SF)
        if masked_result & msb_mask:
            self.value |= (1 << 7)   # Set SF (Negative)
        else:
            self.value &= ~(1 << 7)  # Clear SF (Positive)

        # 3. Parity Flag (PF) - Check least significant byte for even number of 1s
        lsb = masked_result & 0xFF
        ones_count = bin(lsb).count('1')
        if ones_count % 2 == 0:
            self.value |= (1 << 2)   # Set PF (Even parity)
        else:
            self.value &= ~(1 << 2)  # Clear PF (Odd parity)

        # 4. Carry Flag (CF) - Unsigned overflow check
        if not is_subtraction:
            if (operand1 + operand2) > max_val:
                self.value |= (1 << 0)
            else:
                self.value &= ~(1 << 0)
        else:
            if operand1 < operand2:
                self.value |= (1 << 0)
            else:
                self.value &= ~(1 << 0)

        # 5. Overflow Flag (OF) - Signed overflow check
        if not is_subtraction:
            # Positive + Positive = Negative OR Negative + Negative = Positive
            op1_sign = bool(operand1 & msb_mask)
            op2_sign = bool(operand2 & msb_mask)
            res_sign = bool(masked_result & msb_mask)
            if op1_sign == op2_sign and op1_sign != res_sign:
                self.value |= (1 << 11)
            else:
                self.value &= ~(1 << 11)
        else:
            op1_sign = bool(operand1 & msb_mask)
            op2_sign = bool(operand2 & msb_mask)
            res_sign = bool(masked_result & msb_mask)
            if op1_sign != op2_sign and op1_sign != res_sign:
                self.value |= (1 << 11)
            else:
                self.value &= ~(1 << 11)

    def display_flags(self):
        print(f"EFLAGS Register State: 0x{self.value:08X}")
        print(f"  CF (Carry):    {(self.value >> 0) & 1}")
        print(f"  PF (Parity):   {(self.value >> 2) & 1}")
        print(f"  AF (AuxCarry): {(self.value >> 4) & 1}")
        print(f"  ZF (Zero):     {(self.value >> 6) & 1}")
        print(f"  SF (Sign):     {(self.value >> 7) & 1}")
        print(f"  OF (Overflow): {(self.value >> 11) & 1}")


flags = EFlags()

flags.update_arithmetic_flags(result=0, operand1=0, operand2=0)
flags.display_flags()
