#include "chip8.h"

uint8_t fontset[80];

void chip8_init(struct chip8 *cpu)
{
    //Reset memory
    memset(cpu->memory, 0, sizeof(cpu->memory));
    //Reset display
    memset(cpu->display, 0, sizeof(cpu->display));
    //Reset V
    memset(cpu->V, 0, sizeof(cpu->V));
    //Reset stack
    memset(cpu->stack, 0, sizeof(cpu->stack));

    memset(cpu->keypad, 0, sizeof(cpu->keypad));

    cpu->I = 0;
    cpu->sp = 0;
    cpu->delay_timer = 0;
    cpu->sound_timer = 0;

    cpu->pc = CHIP8_PROGRAM_START;

    for (size_t i = 0; i < CHIP8_FONTSET_SIZE; i++) {
       cpu->memory[CHIP8_FONT_START + i] = fontset[i];
    }
}

int chip8_load_rom(struct chip8 *cpu, const char *rom_path)
{
    FILE *rom = fopen(rom_path, "rb");
    
    if (rom == NULL) {
        perror("Error: Cannot open file");
        return EXIT_FAILURE;
    }

    fseek(rom, 0, SEEK_END);
    long rom_size = ftell(rom);

    printf("Rom size is %ld", rom_size);

    if (rom_size > 3584) {
        perror("Error: Rom file is too large");
        fclose(rom);
        return EXIT_FAILURE;
    }

    if (rom_size < 0) {
        perror("Error: ftell failed to read rom");
        fclose(rom);
        return EXIT_FAILURE;
    } 
    else if (rom_size == 0) {
        perror("Error: Empty rom");
        fclose(rom);
        return EXIT_FAILURE;
    }

    rewind(rom);

    unsigned char *buffer = malloc(rom_size);
    if (buffer == NULL) {
        perror("Error: Memory allocation failed");
        fclose(rom);
        return EXIT_FAILURE;
    }

    size_t memory_size = fread(buffer, 1, rom_size, rom);

    if (memory_size != rom_size) {
        perror("Error: Rom size not equal");
        free(buffer);
        fclose(rom);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < rom_size; i++) {
        cpu->memory[0x200 + i] = buffer[i];
    }

    free(buffer);
    fclose(rom);

    return EXIT_SUCCESS;
}

void chip8_cycle(struct chip8 *cpu) 
{
    if (cpu->pc >= CHIP8_MEMORY_SIZE) {
        printf("PC reached end of memory (0x%04X). Halting cycle.\n", cpu->pc);
        return; 
    }
    
    uint8_t byte_1 = cpu->memory[cpu->pc];
    uint8_t byte_2 = cpu->memory[cpu->pc+1];
    uint16_t opcode = (byte_1 << 8) | byte_2;

    cpu->pc = cpu->pc + 2;

    uint16_t nnn = opcode & 0x0FFF;
    uint8_t nn = opcode & 0x00FF;
    uint8_t n = opcode & 0x000F;
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;

    uint16_t firstDigit = (opcode & 0xF000);

    switch (firstDigit) {
    case 0x6000:
        cpu->V[x] = nn;
        break;
    case 0x7000:
        cpu->V[x] += nn;
        break;
    case 0xA000:
        cpu->I = nnn;
        break;
    case 0x1000:
        cpu->pc = nnn;
        break;
    case 0x3000:
        if (cpu->V[x] == nn) {
            cpu->pc += 2;
        }
        break;
    case 0x4000:
        if (cpu->V[x] != nn) {
            cpu->pc += 2;
        }
        break;
    case 0x5000:
        if (n == 0)
            if (cpu->V[x] == cpu->V[y]) {
                cpu->pc += 2;
            }
            else 
                printf("Unknown opcode\n");
        else
            printf("Unknown opcode\n");
        break;
    case 0x0000:
            if (opcode == 0x00E0) {
                memset(cpu->display, 0, sizeof(cpu->display));
            }
            else if (opcode == 0x00EE) {
                cpu->sp -= 1;
                cpu->pc = cpu->stack[cpu->sp];
            }
            else 
                printf("Unknown or unimplemented opcode\n");
            break;
    case 0xD000:
            uint8_t xPos = cpu->V[x];
            uint8_t yPos = cpu->V[y];
            cpu->V[0xF] = 0;

            for (uint8_t row = 0; row < n; row++) {
                uint8_t sprite_byte = cpu->memory[cpu->I + row];

                for (uint8_t bit = 0; bit < 8; bit++) {
                    if (sprite_byte & (0x80 >> bit)) {
                        uint8_t screen_x = (xPos + bit) % 64;
                        uint8_t screen_y = (yPos + row) % 32;
                        uint16_t display_index = screen_y * 64 + screen_x;

                        if (cpu->display[display_index] == 1)
                            cpu->V[0xF] = 1;
                        
                        cpu->display[display_index] = cpu->display[display_index] ^ 1;
                    }
                }
            }
            break;
        case 0x2000:
            cpu->stack[cpu->sp] = cpu->pc;
            cpu->sp += 1;
            cpu->pc = nnn;
            break;
        case 0xC000:
            uint8_t random_num = rand() % 256;
            cpu->V[x] = random_num & nn;
            break;
        case 0x8000:
            switch (n) {
            case 0:
                cpu->V[x] = cpu->V[y];
                break;
            case 1:
                cpu->V[x] = cpu->V[x] | cpu->V[y];
                break;
            case 2:
                cpu->V[x] = cpu->V[x] & cpu->V[y];
                break;
            case 3:
                cpu->V[x] = cpu->V[x] ^ cpu->V[y];
                break;
            case 4:
                uint16_t sum = cpu->V[x] + cpu->V[y];

                if (sum > 255)
                    cpu->V[0xF] = 1;
                else 
                    cpu->V[0xF] = 0;

                cpu->V[x] = sum & 0xFF;
                break; 
            case 5:
                if (cpu->V[x] >= cpu->V[y]) 
                    cpu->V[0xF] = 1;
                else 
                    cpu->V[0xF] = 0;
                
                cpu->V[x] = cpu->V[x] - cpu->V[y];
                break;
            case 6:
                cpu->V[0xF] = cpu->V[x] & 1;
                cpu->V[x] = cpu->V[x] >> 1;
                break;
            case 7:
                if (cpu->V[y] >= cpu->V[x]) 
                    cpu->V[0xF] = 1;
                else 
                    cpu->V[0xF] = 0;
                
                cpu->V[x] = cpu->V[y] - cpu->V[x];
                break;
            case 0xE:
                if (cpu->V[x] & 0x80)
                    cpu->V[0xF] = 1;
                else 
                    cpu->V[0xF] = 0;
                    
                cpu->V[x] = cpu->V[x] << 1;
                break;
            default:
                printf("Unknown 8XY opcode");
                break;
            }
            break;
        case 0x9000:
            if (n == 0)
                if (cpu->V[x] != cpu->V[y]) {
                    cpu->pc += 2;
                }
            else 
                printf("Uknown opcode");
            break;
        case 0xB000:
                cpu->pc = nnn + cpu->V[0];
                break;
        case 0xF000:
            switch (nn) {
            case 0x0A:
                bool keyPressed = false;
                for (size_t key = 0; key <= 15; key++) {
                    if (cpu->keypad[key] != 0) {
                        cpu->V[x] = key;
                        keyPressed = true;
                        break;
                    }
                } 
                if (!keyPressed)
                    cpu->pc -= 2;
                break;
            case 0x07:
                cpu->V[x] = cpu->delay_timer;
                break;
            case 0x15:
                cpu->delay_timer = cpu->V[x];
                break;
            case 0x18:
                cpu->sound_timer = cpu->V[x];
                break;
            case 0x1E:
                cpu->I += cpu->V[x];
                break;
            case 0x29:
                cpu->I = 0x050 + cpu->V[x] * 5;
                break;
            case 0x33:
                uint8_t value = cpu->V[x];
                uint8_t first_num = value / 100;
                uint8_t second_num = (value / 10) % 10;
                uint8_t third_num = value % 10;

                cpu->memory[cpu->I] = first_num;
                cpu->memory[cpu->I + 1] = second_num;
                cpu->memory[cpu->I + 2] = third_num;
                break;
            case 0x55:
                for (size_t index = 0; index <= x; index++) {
                    cpu->memory[cpu->I + index] = cpu->V[index];
                }
                break;
            case 0x65:
                for (size_t index = 0; index <= x; index++) {
                    cpu->V[index] = cpu->memory[cpu->I + index];
                }
                break;
            default:
                printf("Unkown opcode.");
                break;
            }
            break;
        case 0xE000:
            uint8_t key = cpu->V[x];

            if (key > 0xF) {
                printf("Invalid key");
                break;
            }

            switch (nn) {
            case 0x9E:
                if (cpu->keypad[key])
                    cpu->pc += 2;
                break;
            case 0xA1:
                if (!cpu->keypad[key])
                    cpu->pc += 2;
                break;
            default:
                printf("Unknown opcode");
                break;
            }
            break;
    default:
        printf("Nothing set\n");
        break;
    }
}