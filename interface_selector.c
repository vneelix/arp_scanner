#include "arp_scanner.h"

char* select_interface() {
	if (initscr() == NULL) {
		return NULL;
	}
	if (cbreak() != OK || noecho() || OK || keypad(stdscr, true) != OK) {
		endwin();
		return NULL;
	}

	struct if_nameindex* name_index_arr = if_nameindex();
	if (name_index_arr == NULL) {
		endwin();
		return NULL;
	}

	int selected_idx = 0;
	int interfaces_count = 0;
	while (
			name_index_arr[interfaces_count].if_index != 0 && name_index_arr[interfaces_count].if_name != NULL
			) {
		interfaces_count++;
	}

	for (; ;) {
		clear();

		int	idx = 0;
		printw("Available interfaces:\n");
		while (name_index_arr[idx].if_index != 0 && name_index_arr[idx].if_name != NULL) {
			printw("%c %s\n", (selected_idx == idx ? '*' : ' '), name_index_arr[idx].if_name);
			idx++;
		}

		int key_code = getch();
		switch (key_code) {
			case ERR:
				endwin();
				return NULL;
			case KEY_UP:
				selected_idx = selected_idx > 0 ? selected_idx - 1 : selected_idx;
				break;
			case KEY_DOWN:
				selected_idx = selected_idx + 1 < interfaces_count ? selected_idx + 1 : selected_idx;
				break;
			default:
				if (key_code == KEY_ENTER || key_code == 10 || key_code == 13) {
					char* interface_name = strdup(name_index_arr[selected_idx].if_name);
					if_freenameindex(name_index_arr);
					endwin();
					return interface_name;
				}
				break;
		}
	}
}
