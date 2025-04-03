#!/usr/bin/env python3
#
# Fichier de tests pour la phase 2 du projet.
#
# Vous pouvez tester votre projet avec la commande :
#
#   python3 tests2.py chemin/vers/votre/executable

import os
import subprocess
import sys



PROGRAM_FILE = "prog.txt"

PROGRAMS = [
"""SET a 5
ADD a 6
PRINT a
SUB a 4
IFNZ a
PRINT a
SUB a 10
IFNZ a
PRINT a
""",

"""SET a 0
IFNZ a
PRINT a
SET a 5
IFNZ a
PRINT a
""",

"""SET c 50000
PRINT c
ADD c 50000
PRINT c
""",

"""PRINT a
PRINT b
PRINT c
PRINT d
""",

"""SET a 120
ADD a 30
PRINT a
SET b 130
PRINT b
SUB a b
PRINT a
""",

"""SET a 5
PUSH a
ADD a a
PUSH a
SUB a 20
PUSH a
POP d
PRINT d
POP b
PRINT b
POP c
PRINT c
""",

"""SET a 5
SET b 8
STORE 100 a
STORE 102 b
LOAD 100 d
PRINT d
LOAD 102 c
PRINT c
""",

"""SET a 150
SET b 130
PUSH a
ADD a b
PUSH a
STORE 100 a
STORE 101 b
POP c
PRINT c
POP d
PRINT d
LOAD 100 b
IFNZ b
PRINT b
LOAD 101 a
PRINT a
SUB b b
IFNZ b
PRINT b
""",
]

OUTPUTS = [
  [
    ["11"],
    ["7"],
  ],
  [
    ["5"],
  ],
  [
    ["50000"],
    ["65535"],
  ],
  [
    ["0"],
    ["0"],
    ["0"],
    ["0"],
  ],
  [
    ["150"],
    ["130"],
    ["20"],
  ],
  [
    ["0"],
    ["10"],
    ["5"],
  ],
  [
    ["5"],
    ["8"],
  ],
  [
    ["280", "280"],
    ["150", "150"],
    ["256", "33304"],
    ["130", "130"],
  ],
]



def write_program(n: int):
  with open(PROGRAM_FILE, "w+") as f:
    f.write(PROGRAMS[n])


def remove_program():
  os.remove(PROGRAM_FILE)


def check_program_output(n: int):
  program = subprocess.run([sys.argv[1], PROGRAM_FILE], capture_output=True, encoding="utf-8")
  output = program.stdout.split('\n')[:-1]

  if len(output) > len(OUTPUTS[n]):
    OUTPUTS[n].append('')
  elif len(OUTPUTS[n]) > len(output):
    output.append('')

  for output_line, expected_line in zip(output, OUTPUTS[n], strict=True):
    if output_line not in expected_line:
      print(f"### FAIL ###   Expected {expected_line} but output is '{output_line}'", file=sys.stderr)
      print(program.stderr, file=sys.stderr)
      return False

  return 



write_program(5)
    
class Processor:
    @staticmethod
    def saturate(value: int) -> int:
        if value < 0:
            return 0
        if value > 65535:
            return 65535
        return value

    @staticmethod
    def parse_instruction(instr: str) -> list[str]:
        return instr.split()

    @staticmethod
    def is_valid_register(reg: str) -> bool:
        return reg in {"a", "b", "c", "d"}

    def exec(self, program_path: str) -> None:
        registers = [0] * 4  # a=0, b=1, c=2, d=3
        skip_next = False

        with open(program_path, 'r') as file:
            for line in file:
                line = line.strip()
                if skip_next:
                    skip_next = False
                    continue

                tokens = self.parse_instruction(line)
                if not tokens:
                    continue

                opcode = tokens[0]

                if opcode == "SET" and len(tokens) >= 3:
                    reg = tokens[1]
                    if not self.is_valid_register(reg):
                        continue
                    
                    value = int(tokens[2])
                    reg_index = ord(reg) - ord('a')
                    registers[reg_index] = self.saturate(value)
                
                elif opcode == "ADD" and len(tokens) >= 3:
                    reg_dest = tokens[1]
                    if not self.is_valid_register(reg_dest):
                        continue
                    
                    reg_dest_index = ord(reg_dest) - ord('a')
                    
                    if self.is_valid_register(tokens[2]):
                        reg_src = tokens[2]
                        reg_src_index = ord(reg_src) - ord('a')
                        registers[reg_dest_index] = self.saturate(
                            registers[reg_dest_index] + registers[reg_src_index]
                        )
                    else:
                        value = int(tokens[2])
                        registers[reg_dest_index] = self.saturate(
                            registers[reg_dest_index] + value
                        )
                
                elif opcode == "SUB" and len(tokens) >= 3:
                    reg_dest = tokens[1]
                    if not self.is_valid_register(reg_dest):
                        continue
                    
                    reg_dest_index = ord(reg_dest) - ord('a')
                    
                    if self.is_valid_register(tokens[2]):
                        reg_src = tokens[2]
                        reg_src_index = ord(reg_src) - ord('a')
                        registers[reg_dest_index] = self.saturate(
                            registers[reg_dest_index] - registers[reg_src_index]
                        )
                    else:
                        value = int(tokens[2])
                        registers[reg_dest_index] = self.saturate(
                            registers[reg_dest_index] - value
                        )
                
                elif opcode == "PRINT" and len(tokens) >= 2:
                    reg = tokens[1]
                    if not self.is_valid_register(reg):
                        continue
                    
                    reg_index = ord(reg) - ord('a')
                    print(f"['{registers[reg_index]}']")  # Format exact avec apostrophes
                
                elif opcode == "IFNZ" and len(tokens) >= 2:
                    reg = tokens[1]
                    if not self.is_valid_register(reg):
                        continue
                    
                    reg_index = ord(reg) - ord('a')
                    if registers[reg_index] == 0:
                        skip_next = True
procesor = Processor()
procesor.exec(PROGRAM_FILE)