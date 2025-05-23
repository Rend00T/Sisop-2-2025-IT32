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


### Error Handling 
### Unzip Error 
![Image](https://github.com/user-attachments/assets/90870ec6-d758-4097-b27e-47aec6626881)
Error disebabkan oleh pada bagian code seharusnya dimasukkan link google drive jadi tidak hanya mendownload file yang bernama "Clues.zip" tetapi mendownload link drive nya seperti pada bagian code berikut.

![Image](https://github.com/user-attachments/assets/13c79399-159e-4d7a-957d-75962f0ef67f)
dan sehingga menghasilkan hasil sebagai berikut ketika menjalankan menggunakan ./action.  
![Image](https://github.com/user-attachments/assets/9f54cc2d-f4ef-49d4-bc8f-5ee99f865696)

# Soal 2
Solved by. 057_Ananda Fitri Wibowo
## A. Sebelum Revisi
### a. Mengetahui semua aktivitas user (list)
Mendapatkan PID, command, CPU usage, dan memory usage dari /proc.
```
void list_processes(const char *user) {
    int uid = get_uid(user);
    if (uid < 0) return;
    DIR *d = opendir("/proc");
    struct dirent *e;
    printf("PID\tCMD\tCPU\tMEM\n");
    while ((e = readdir(d))) {
        if (!is_numeric(e->d_name)) continue;
        char path[512];
        if (snprintf(path, sizeof(path), "/proc/%s/status", e->d_name) >= sizeof(path)) continue;
        FILE *f = fopen(path, "r");
        if (!f) continue;
        char line[256]; int p_uid = -1;
        while (fgets(line, sizeof(line), f))
            if (sscanf(line, "Uid:\t%d", &p_uid) == 1) break;
        fclose(f);
        if (p_uid != uid) continue;

        char cmd[64] = "[gak ketauan]";
        snprintf(path, sizeof(path), "/proc/%s/cmdline", e->d_name);
        f = fopen(path, "r");
        if (f) {
            fread(cmd, 1, sizeof(cmd) - 1, f);
            fclose(f);
        }

        float cpu = 0.0, mem = 0.0;
        snprintf(path, sizeof(path), "/proc/%s/stat", e->d_name);
        f = fopen(path, "r");
        if (f) {
            long utime, stime;
            int dummy; char comm[64], state;
            fscanf(f, "%d %s %c", &dummy, comm, &state);
            for (int i = 0; i < 11; i++) fscanf(f, "%*s");
            fscanf(f, "%ld %ld", &utime, &stime);
            cpu = (utime + stime) / 100.0;
            fclose(f);
        }

        snprintf(path, sizeof(path), "/proc/%s/status", e->d_name);
        f = fopen(path, "r");
        if (f) {
            while (fgets(line, sizeof(line), f)) {
                if (sscanf(line, "VmRSS: %f", &mem) == 1) {
                    mem /= 1024.0;
                    break;
                }
            }
            fclose(f);
        }

        printf("%s\t%s\t%.1f\t%.1f MB\n", e->d_name, cmd, cpu, mem);
    }
    closedir(d);
}
```

### b. Memasang mata-mata untuk mengawasi user (daemon)
Bikin fork dan ngasih stamp ke log setiap 10 detik, menandakan user sedang diawasi.
```
void daemon_mode(const char *user) {
    pid_t pid = fork();
    if (pid < 0) return;
    if (pid > 0) {
        printf("[DEBUGMON] hehehhe lu gw awasin banh! '%s'~\n", user);
        return;
    }
    setsid();
    fclose(stdin); fclose(stdout); fclose(stderr);
    while (1) {
        log_status(user, "RUNNING");
        sleep(10);
    }
}
```

### c. Menghentikan pengawasan (stop)
(harusnya) Mematikan daemon yang menjadi pengawas user. Tapi di sini bukannya ngekill daemon, saya malah ngekill proses user, jadi terminal langsung close kalau stop dijalankan.
```
void kill_processes(const char *user, const char *status) {
    int uid = get_uid(user);
    if (uid < 0) return;
    DIR *d = opendir("/proc");
    struct dirent *e;
    while ((e = readdir(d))) {
        if (!is_numeric(e->d_name)) continue;
        char path[512];
        if (snprintf(path, sizeof(path), "/proc/%s/status", e->d_name) >= sizeof(path)) continue;
        FILE *f = fopen(path, "r");
        if (!f) continue;
        char line[128]; int p_uid = -1;
        while (fgets(line, sizeof(line), f))
            if (sscanf(line, "Uid:\t%d", &p_uid) == 1) break;
        fclose(f);
        if (p_uid != uid) continue;
        kill(atoi(e->d_name), SIGKILL);
        log_status(e->d_name, status);
    }
    closedir(d);
    if (strcmp(status, "FAILED") == 0)
        printf("[DEBUGMON] semua proses '%s' udah diberesin (FAILED).\n", user);
    else if (strcmp(status, "RUNNING") == 0)
        printf("[DEBUGMON] proses '%s' dihentikan sementara.\n", user);
}
```

### d. Menghentikan akses user (fail)
(harusnya) Mencabut akses user dalam melakukan apa-apa di terminal, user tidak bisa menjalankan perintah. Tapi seperti stop tadi, saya malah matiin proses user, terminal langsung nutup.
```
void kill_processes(const char *user, const char *status) {
    int uid = get_uid(user);
    if (uid < 0) return;
    DIR *d = opendir("/proc");
    struct dirent *e;
    while ((e = readdir(d))) {
        if (!is_numeric(e->d_name)) continue;
        char path[512];
        if (snprintf(path, sizeof(path), "/proc/%s/status", e->d_name) >= sizeof(path)) continue;
        FILE *f = fopen(path, "r");
        if (!f) continue;
        char line[128]; int p_uid = -1;
        while (fgets(line, sizeof(line), f))
            if (sscanf(line, "Uid:\t%d", &p_uid) == 1) break;
        fclose(f);
        if (p_uid != uid) continue;
        kill(atoi(e->d_name), SIGKILL);
        log_status(e->d_name, status);
    }
    closedir(d);
    if (strcmp(status, "FAILED") == 0)
        printf("[DEBUGMON] semua proses '%s' udah diberesin (FAILED).\n", user);
    else if (strcmp(status, "RUNNING") == 0)
        printf("[DEBUGMON] proses '%s' dihentikan sementara.\n", user);
}
```

### e. Mengizinkan kembali user (revert)
(harusnya) Memberikan akses kembali ke user. Tapi karena fail saya masih error, di sini dia cuma nge-print "Kembali normal".
```
void revert_mode(const char *user) {
    printf("[DEBUGMON] oke, dah aman '%s', lanjut lanjut\n", user);
    log_status(user, "RUNNING");
}
```

### f. Mencatat semua ke log (daemon.log)
Mencatat semua aktivitas yang berjalan.
```
void log_status(const char *proc, const char *status) {
    FILE *fp = fopen(LOG_FILE, "a");
    if (!fp) return;
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    fprintf(fp, "[%02d:%02d:%04d]-[%02d:%02d:%02d]_%s_%s\n",
        t->tm_mday, t->tm_mon+1, t->tm_year+1900,
        t->tm_hour, t->tm_min, t->tm_sec, proc, status);
    fclose(fp);
}
```

### g. Dokumentasi
![image](https://github.com/user-attachments/assets/41f6ca9d-4dc0-4215-916f-666d56e356d2)
*stop dan fail menutup terminal

## 2. Setelah Revisi
Memperbaiki Stop, Fail, dan Revert.
### a. Memperbaiki stop
Sekarang, stop akan membunuh proses daemon, jadi mata-mata menghilang.
```
void stop_daemon(const char *user) {
    char path[128];
    snprintf(path, sizeof(path), "/tmp/debugmon_daemon_%s", user);
    FILE *fp = fopen(path, "r");
    if (!fp) return;
    pid_t pid;
    fscanf(fp, "%d", &pid);
    fclose(fp);
    if (kill(pid, SIGTERM) == 0) {
        printf("[DEBUGMON] daemon '%s' udah dihentikan.\n", user);
        remove(path);
        log_status(user, "RUNNING");
    }
}
```

### b. Memperbaiki Fail
Failnnya masih belum bisa, ada opsi untuk menggunakan root, namun saya lihat di soal shift 2 tidak ada perintah untuk menggunakan sudo saat menjalankan debugmon.
```
void fail_daemon(const char *user) {
    pid_t pid = fork();
    if (pid < 0) return;
    if (pid > 0) return; // parent
    setsid();
    while (1) {
        DIR *d = opendir("/proc");
        struct dirent *e;
        int uid = get_uid(user);
        while ((e = readdir(d))) {
            if (!is_numeric(e->d_name)) continue;
            char path[512];
            snprintf(path, sizeof(path), "/proc/%s/status", e->d_name);
            FILE *f = fopen(path, "r");
            if (!f) continue;
            char line[128]; int p_uid = -1;
            while (fgets(line, sizeof(line), f)) {
                if (sscanf(line, "Uid:\t%d", &p_uid) == 1) break;
            }
            fclose(f);
            if (p_uid != uid) continue;
            int pid = atoi(e->d_name);
            if (pid == getpid()) continue;
            kill(pid, SIGKILL);
            log_status(e->d_name, "FAILED");
        }
        closedir(d);
        sleep(2);
    }
}

void lock_user(const char *user) {
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "pkill -u %s", user);
    system(cmd);

    char bashrc[256];
    snprintf(bashrc, sizeof(bashrc), "/home/%s/.bashrc", user);
    FILE *fp = fopen(bashrc, "a");
    if (fp) {
        fprintf(fp, "echo 'Terminal kamu dikunci oleh debugmon.'\nwhile true; do sleep 1; done\n");
        fclose(fp);
    }

    log_status(user, "FAILED");
    printf("[DEBUGMON] semua proses '%s' udah diberesin (FAILED).\n", user);
    fail_daemon(user);
}
```

### c. Memperbaiki Revert
Kalau fail berhasil, sepertinya ini harusnya work.
```
void unlock_user(const char *user) {
    char bashrc[256];
    snprintf(bashrc, sizeof(bashrc), "/home/%s/.bashrc", user);
    FILE *fp = fopen(bashrc, "r");
    if (!fp) return;
    FILE *tmp = fopen("/tmp/.bashrc_tmp", "w");
    if (!tmp) {
        fclose(fp);
        return;
    }
    char line[512];
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, "Terminal kamu dikunci") || strstr(line, "while true"))
            continue;
        fputs(line, tmp);
    }
    fclose(fp);
    fclose(tmp);
    rename("/tmp/.bashrc_tmp", bashrc);
    printf("[DEBUGMON] oke, dah aman '%s', lanjut lanjut\n", user);
    log_status(user, "RUNNING");
}
```

### d. Dokumentasi
![image](https://github.com/user-attachments/assets/46627bcb-4d4e-442d-a425-c825aa88ab4a)

## C. Error Handling
Perintah yang dapat dipakai untuk menjalankan Debugmon.
~~~
./debugmon list <user>
./debugmon daemon <user>
./debugmon stop <user>
./debugmon fail <user>
./debugmon revert <user>
cat debugmon.log
~~~
