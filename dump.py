from __future__ import annotations
import subprocess
import struct

def read_memory(pid, address, size):
    with open(f"/proc/{pid}/mem", "rb") as mem:
        mem.seek(address)
        return mem.read(size)

def find_module(pid, name):
    with open(f"/proc/{pid}/maps", "r") as maps:
        for line in maps:
            if name in line:
                addr = line.split('-')[0]
                return int(addr, 16)
    return None

class List:
    def __init__(self, pid: int, base: int, elem_size: int) -> None:
        self._pid = pid
        self.__base = base
        self.__elem_size = elem_size

    def get(self, index: int) -> bytes:
        if index < 0:
            raise IndexError("Index must be non-negative")
        address = self.__base + index * self.__elem_size
        return read_memory(self._pid, address, self.__elem_size)

class EntityList(List):
    def get(self, index: int) -> Entity:
        return Entity(self._pid, int.from_bytes(super().get(index), "little"))

class Entity:
    def __init__(self, pid: int, base: int) -> None:
        self._pid = pid
        self.__base = base

    @property
    def x(self) -> float:
        data = read_memory(self._pid, self.__base + 36, 4)
        return struct.unpack("<f", data)[0]

    @property
    def y(self) -> float:
        data = read_memory(self._pid, self.__base + 36 + 4, 4)
        return struct.unpack("<f", data)[0]

    @property
    def z(self) -> float:
        data = read_memory(self._pid, self.__base + 36 + 8, 4)
        return struct.unpack("<f", data)[0]

    @property
    def health(self) -> float:
        data = read_memory(self._pid, self.__base + 36 + 0xec, 4)
        return struct.unpack("<f", data)[0]

def main():
    pid = subprocess.getoutput("pidof SeriousSam").split()[0]
    base = find_module(pid, "libEntities.so")
    if base is None:
        print("Module not found")
        return

    # entity_list = EntityList(int(pid), base + 0x2dee00, 8)

    # for i in range(2):
    #     print(f"Entity {i}:")
    #     entity = entity_list.get(i)
    #     print(f"  Position: ({entity.x}, {entity.y}, {entity.z})")
    #     print(f"  Health: {entity.health}")

    # read 16 floats at 555f113c7bb1 and print as matrix
    address = 0x7fff53b1ca50  # second to last float in the matrix
    # address = &matrix[3][2]
    # address - 4 = &matrix[3][1]
    # address - 8 = &matrix[3][0]
    # address - 12 = &matrix[2][3]
    # address - 16 = &matrix[2][2]
    # address - 20 = &matrix[2][1]
    # address - 24 = &matrix[2][0]
    # address - 28 = &matrix[1][3]
    # address - 32 = &matrix[1][2]
    # address - 36 = &matrix[1][1]
    # address - 40 = &matrix[1][0]
    # address - 44 = &matrix[0][3]
    # address - 48 = &matrix[0][2]
    # address - 52 = &matrix[0][1]
    # address - 56 = &matrix[0][0]
    address -= 56  # Adjust to the start of the matrix

    data = read_memory(int(pid), address, 16 * 4)
    matrix = [struct.unpack("<f", data[i:i+4])[0] for i in range(0, len(data), 4)]
    print("Matrix:")
    for i in range(4):
        for j in range(4):
            prefix = " " if matrix[i*4+j] >= 0 else ""
            print(f"{prefix}{matrix[i*4+j]:.2f}", end=" ")
        print()

if __name__ == "__main__":
    main()
