#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include <sys/stat.h>
#include <unistd.h>

#define CLUES_FOLDER "Clues"
#define FILTERED_FOLDER "Filtered"
#define COMBINED_FILE "Combined.txt"
#define DECODED_FILE "Decoded.txt"
#define ZIP_FILE "Clues.zip"

int exists(const char *path) {
    struct stat st;
    return stat(path, &st) == 0;
}

void create_filtered_folder() {
    struct stat st = {0};
    if (stat(FILTERED_FOLDER, &st) == -1) {
        mkdir(FILTERED_FOLDER, 0700);
    }
}

int is_valid_filename(const char *name) {
    return strlen(name) == 5 && name[1] == '.' && name[2] == 't' && name[3] == 'x' && name[4] == 't' &&
           ((name[0] >= '0' && name[0] <= '9') || (name[0] >= 'a' && name[0] <= 'z'));
}

void setup_clues() {
    if (exists(CLUES_FOLDER)) {
        printf("Folder Clues sudah ada. Melewati proses unduh.\n");
        return;
    }

    printf("Mengunduh Clues.zip dari Google Drive...\n");

    const char *gdrive_download_link = "https://drive.google.com/uc?export=download&id=1xFn1OBJUuSdnApDseEczKhtNzyGekauK";

    char cmd[512];
    snprintf(cmd, sizeof(cmd), "wget -q --show-progress -O %s \"%s\"", ZIP_FILE, gdrive_download_link);
    if (system(cmd) != 0) {
        fprintf(stderr, "Gagal mengunduh Clues.zip\n");
        return;
    }

    printf("Ekstraksi Clues.zip...\n");
    if (system("unzip -q Clues.zip") != 0) {
        fprintf(stderr, "Gagal mengekstrak Clues.zip\n");
        return;
    }

    remove(ZIP_FILE);
    printf("Selesai mengunduh dan mengekstrak Clues.zip\n");
}

void filter_files() {
    create_filtered_folder();

    DIR *clues_dir = opendir(CLUES_FOLDER);
    if (!clues_dir) {
        perror("Gagal membuka direktori Clues");
        return;
    }

    struct dirent *folder_entry;
    while ((folder_entry = readdir(clues_dir)) != NULL) {
        if (folder_entry->d_type != DT_DIR || strcmp(folder_entry->d_name, ".") == 0 || strcmp(folder_entry->d_name, "..") == 0)
            continue;

        char subfolder_path[256];
        snprintf(subfolder_path, sizeof(subfolder_path), "%s/%s", CLUES_FOLDER, folder_entry->d_name);
        DIR *subfolder = opendir(subfolder_path);
        if (!subfolder) continue;

        struct dirent *file_entry;
        while ((file_entry = readdir(subfolder)) != NULL) {
            if (file_entry->d_type == DT_REG && is_valid_filename(file_entry->d_name)) {
                char src[512], dest[512];
                snprintf(src, sizeof(src), "%s/%s", subfolder_path, file_entry->d_name);
                snprintf(dest, sizeof(dest), "%s/%s", FILTERED_FOLDER, file_entry->d_name);

                FILE *f_src = fopen(src, "r");
                FILE *f_dest = fopen(dest, "w");
                if (f_src && f_dest) {
                    int ch;
                    while ((ch = fgetc(f_src)) != EOF)
                        fputc(ch, f_dest);
                }
                if (f_src) fclose(f_src);
                if (f_dest) fclose(f_dest);
            }
        }
        closedir(subfolder);
    }
    closedir(clues_dir);

    clues_dir = opendir(CLUES_FOLDER);
    while ((folder_entry = readdir(clues_dir)) != NULL) {
        if (folder_entry->d_type != DT_DIR || strcmp(folder_entry->d_name, ".") == 0 || strcmp(folder_entry->d_name, "..") == 0)
            continue;

        char subfolder_path[256];
        snprintf(subfolder_path, sizeof(subfolder_path), "%s/%s", CLUES_FOLDER, folder_entry->d_name);
        DIR *subfolder = opendir(subfolder_path);
        if (!subfolder) continue;

        struct dirent *file_entry;
        while ((file_entry = readdir(subfolder)) != NULL) {
            if (file_entry->d_type == DT_REG && !is_valid_filename(file_entry->d_name)) {
                char path[512];
                snprintf(path, sizeof(path), "%s/%s", subfolder_path, file_entry->d_name);
                remove(path);
            }
        }
        closedir(subfolder);
    }
}

int compare(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

void combine_files() {
    FILE *combined = fopen(COMBINED_FILE, "w");
    if (!combined) {
        perror("Gagal membuat Combined.txt");
        return;
    }

    DIR *dir = opendir(FILTERED_FOLDER);
    if (!dir) return;

    char *files[100];
    int file_count = 0;

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type != DT_REG)
            continue;
        files[file_count++] = strdup(entry->d_name);
    }
    closedir(dir);

    qsort(files, file_count, sizeof(char *), compare);

    char *digit_files[100], *char_files[100];
    int digit_count = 0, char_count = 0;

    for (int i = 0; i < file_count; i++) {
        if (isdigit(files[i][0])) {
            digit_files[digit_count++] = files[i];
        } else if (isalpha(files[i][0])) {
            char_files[char_count++] = files[i];
        } else {
            free(files[i]);
        }
    }

    int i = 0, j = 0;
    while (i < digit_count || j < char_count) {
        if (i < digit_count) {
            char path[512];
            snprintf(path, sizeof(path), "%s/%s", FILTERED_FOLDER, digit_files[i]);
            FILE *f = fopen(path, "r");
            if (f) {
                int ch = fgetc(f);
                if (ch != EOF) fputc(ch, combined);
                fclose(f);
                remove(path);
            }
            free(digit_files[i]);
            i++;
        }
        if (j < char_count) {
            char path[512];
            snprintf(path, sizeof(path), "%s/%s", FILTERED_FOLDER, char_files[j]);
            FILE *f = fopen(path, "r");
            if (f) {
                int ch = fgetc(f);
                if (ch != EOF) fputc(ch, combined);
                fclose(f);
                remove(path);
            }
            free(char_files[j]);
            j++;
        }
    }

    fclose(combined);
}

char rot13(char ch) {
    if ('a' <= ch && ch <= 'z') return 'a' + (ch - 'a' + 13) % 26;
    if ('A' <= ch && ch <= 'Z') return 'A' + (ch - 'A' + 13) % 26;
    return ch;
}

void decode_file() {
    FILE *in = fopen(COMBINED_FILE, "r");
    FILE *out = fopen(DECODED_FILE, "w");

    if (!in || !out) {
        perror("Gagal membuka Combined.txt atau membuat Decoded.txt");
        if (in) fclose(in);
        if (out) fclose(out);
        return;
    }

    int ch;
    while ((ch = fgetc(in)) != EOF) {
        fputc(rot13(ch), out);
    }

    fclose(in);
    fclose(out);
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        setup_clues(); 
        return 0;
    }

    if (argc == 3 && strcmp(argv[1], "-m") == 0) {
        if (strcmp(argv[2], "Filter") == 0) {
            filter_files();
        } else if (strcmp(argv[2], "Combine") == 0) {
            combine_files();
        } else if (strcmp(argv[2], "Decode") == 0) {
            decode_file();
        } else {
            fprintf(stderr, "Mode tidak valid. Gunakan Filter, Combine, atau Decode.\n");
            return 1;
        }
    } else {
        fprintf(stderr, "Usage:\n  %s           (untuk download dan ekstrak Clues)\n", argv[0]);
        fprintf(stderr, "  %s -m [Filter|Combine|Decode]\n", argv[0]);
        return 1;
    }

    return 0;
}
