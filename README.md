# Sisop-2-2025-IT32
- Alif Bima Permadani (5027241020)
- Ananda Fitri Wibowo (5027241057)
- Raihan Fahri Ghazali (5027241061)

| No | Nama                   | NRP         |
|----|------------------------|-------------|
| 1  | Alif Bima Permadani    | 5027241020  |
| 2  | Ananda Fitri Wibowo    | 5027241057  |
| 3  | Raihan Fahri Ghazali   | 5027241061  |

# Soal 1
Solver by. 061_Raihan Fahri Ghazali
## a. Downloading the Clues
Pada soal ini diperintahkan untuk mendownload, unzip, dan kemudian menghapus Clues.zip

### DOWNLOAD 
Intuk mendownload file lewat link yang dikasih, saya menggunakan CURL
```
const char *gdrive_download_link = "https://drive.google.com/uc?export=download&id=1xFn1OBJUuSdnApDseEczKhtNzyGekauK";

char cmd[512];
snprintf(cmd, sizeof(cmd), "wget -q --show-progress -O %s \"%s\"", ZIP_FILE, gdrive_download_link);
if (system(cmd) != 0) {
    fprintf(stderr, "Gagal mengunduh Clues.zip\n");
    return;
}
```

### UNZIP 
Untuk mengunzip fole nya saya menggunakan perintah unzip pada file Clues.zip
```
if (system("unzip -q Clues.zip") != 0) {
    fprintf(stderr, "Gagal mengekstrak Clues.zip\n");
    return;
}
```

###REMOVE Clues.zip
```
remove(ZIP_FILE);
    printf("Selesai mengunduh dan mengekstrak Clues.zip\n");
}
```
Berikut adalah code yang saya pakai untuk menghapus file Clues.zip dan jika berhasil akan mengoutput hasil printf nya.


## b. Filtering the Files
PErtama tama kita coba buatkan folder "Filtered" jika masih blom dia akan dibuatkan menggunakan mkdir pada code ini 
```
void create_filtered_folder() {
    struct stat st = {0};
    if (stat(FILTERED_FOLDER, &st) == -1) {
        mkdir(FILTERED_FOLDER, 0700);
    }
}
```
Pada Code function ini bertugas untuk Memfilter file dalam direktori Clues berdasarkan pola nama berkas yang valid. Memindahkan berkas yang valid ke folder Filtered dan menghapus berkas yang tidak valid.

```
int is_valid_filename(const char *name) {
    return strlen(name) == 5 && name[1] == '.' && name[2] == 't' && name[3] == 'x' && name[4] == 't' &&
           ((name[0] >= '0' && name[0] <= '9') || (name[0] >= 'a' && name[0] <= 'z'));
}
```
Pada bagian code ini bertugas untuk memvalidasi apakah sebuah nama file sesuai dengan kriteria yang ditentukan dalam program. Nah, selanjut kita masuk pada bagian function untuk memfilter nya 

```
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
```
Fungsi pada code ini bertujuan untuk memeriksa setiap file dalam subfolder di dalam direktori Clues dan, jika file tersebut valid, menyalinnya ke folder Filtered.


## c. Combine the File Content
Pada bagian ini diperintahkan untuk menggabungkan isi file dengan urutan huruf lalu angka kemudian huruf dan seterusnya. Setelah itu file pada folder sebelumnya dihapus.

```
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
                fclose
}
```
Sedangkan pada function ini untuk menggabungkan file menjadi satu file teks (Combined.txt) ketika menjalankan perintah ./action -m Combine ada pada fungsi combine_files(). Fungsi ini akan membaca semua file dari folder Filtered, mengurutkan file-file tersebut, dan menggabungkan isinya ke dalam satu file bernama Combined.txt.

```
int i = 0, j = 0;
while (i < digit_count || j < char_count) {
    if (i < digit_count) {
        ...
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
        ...
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
```
Pada keselurahn dari kode ini untuk menggabungkan dan mengurutkan file yang ada di dua grup berbeda, yakni file yang diawali dengan angka dan file yang diawali dengan huruf, dan kemudian menggabungkan isi file-file tersebut ke dalam satu file output (combined).

## d. Decode the File 
```
char rot13(char ch) {
    if ('a' <= ch && ch <= 'z') return 'a' + (ch - 'a' + 13) % 26;
    if ('A' <= ch && ch <= 'Z') return 'A' + (ch - 'A' + 13) % 26;
    return ch;
}
```
Sekarang kita masuk ke perintah terakhir untuk Decode the File menggunakan algortima ROT13 yang pada bagian code ini igunakan untuk mengenkripsi dan mendekripsi huruf dalam teks. Sedangakan, ROT13 adalah algoritma substitusi sederhana yang menggeser setiap huruf dalam abjad sebanyak 13 posisi.

```
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
```
lalu untuk function yang terakhir bertugas untuk membaca file yang telah dienkripsi (misalnya COMBINED_FILE), mendekripsi isinya menggunakan algoritma ROT13, dan menyimpannya dalam file baru (misalnya DECODED_FILE). Untuk menjalan Decode nya kita menggunakan ./action -m Decode 

## e. Password Check
Setelah melihat hasil dari Decoded.txt, selanjutnya output dimasukkan kedalam Lokasi Password Check dan hasilnya sebagai berikut.
![image](https://github.com/user-attachments/assets/64b30716-0f6d-41e1-9595-bfb2d40b7522)


###Error Handling 
### Unzip Error 
![image](https://github.com/Rend00T/Sisop-2-2025-IT32/issues/1#issue-3004893788)
