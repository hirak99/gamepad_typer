#include <SDL2/SDL.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

const int keyboardWidth = 10;
const int keyboardHeight = 4;

// Simple virtual keyboard layout
const char* kKeysArray[keyboardHeight][keyboardWidth] = {
    {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0"},
    {"Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P"},
    {"A", "S", "D", "F", "G", "H", "J", "K", "L", " "},
    {"Z", "X", "C", "V", "B", "N", "M", "<-", "Enter", "Quit"}};

void DisplayKeyboard(int cursor_x, int cursor_y,
                     const std::string& current_text) {
  // system("clear");  // Clear the terminal (use "cls" on Windows)
  std::cerr
      << "\033[2J\033[1;1H";  // Clear the screen and move cursor to top-left
  std::string masked_text(current_text.length(), '*');
  std::cerr << "Type with gamepad: " << masked_text << std::endl;
  for (int i = 0; i < keyboardHeight; ++i) {
    for (int j = 0; j < keyboardWidth; ++j) {
      if (i == cursor_y && j == cursor_x) {
        std::cerr << "[" << kKeysArray[i][j] << "] ";
      } else {
        std::cerr << kKeysArray[i][j] << "  ";
      }
    }
    std::cerr << std::endl;
  }
}

std::string GetUserText() {
  std::string input_text;
  SDL_Event event;
  int cursor_x = 0, cursor_y = 0;
  bool running = true;

  DisplayKeyboard(cursor_x, cursor_y, input_text);

  while (running) {
    SDL_WaitEvent(&event);
    if (event.type == SDL_QUIT) {
      running = false;
      break;
    }
    if (event.type == SDL_CONTROLLERBUTTONDOWN) {
      if (event.cbutton.button == SDL_CONTROLLER_BUTTON_A) {
        const char* selectedChar = kKeysArray[cursor_y][cursor_x];
        if (strcmp(selectedChar, "<-") == 0) {
          if (!input_text.empty()) input_text.pop_back();
        } else if (strcmp(selectedChar, "Enter") == 0) {
          running = false;
        } else if (strcmp(selectedChar, "Quit") == 0) {
          running = false;
        } else {
          input_text += selectedChar;
        }
        DisplayKeyboard(cursor_x, cursor_y, input_text);
      } else if (event.cbutton.button == SDL_CONTROLLER_BUTTON_B) {
        running = false;
      }
      // D-Pad navigation
      if (event.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP &&
          cursor_y > 0) {
        cursor_y--;
      }
      if (event.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN &&
          cursor_y < keyboardHeight - 1) {
        cursor_y++;
      }
      if (event.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_LEFT &&
          cursor_x > 0) {
        cursor_x--;
      }
      if (event.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT &&
          cursor_x < keyboardWidth - 1) {
        cursor_x++;
      }
      DisplayKeyboard(cursor_x, cursor_y, input_text);
    }
  }

  return input_text;
}

int main(int argc, char* argv[]) {
  if (SDL_Init(SDL_INIT_GAMECONTROLLER | SDL_INIT_EVENTS) < 0) {
    std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError()
              << std::endl;
    return 1;
  }

  if (SDL_NumJoysticks() <= 0) {
    std::cerr << "No joysticks detected" << std::endl;
    return 1;
  }

  SDL_GameController* controller = SDL_GameControllerOpen(0);
  if (!controller) {
    std::cerr << "Could not open game controller! SDL_Error: " << SDL_GetError()
              << std::endl;
    SDL_Quit();
    return 1;
  }

  // // Redirect std::cout to a string stream to suppress prompts during user
  // input std::stringstream output; std::streambuf* original_cout_buffer =
  // std::cout.rdbuf(output.rdbuf());

  std::cout << GetUserText();

  // std::cout.rdbuf(original_cout_buffer);

  SDL_GameControllerClose(controller);
  SDL_Quit();
}
