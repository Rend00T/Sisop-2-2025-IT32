#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <pwd.h>
#include <ctype.h>
#include <signal.h>
#include <time.h>

#define LOG_FILE "debugmon.log"

void log_status(const char *proc, const char *status) {
    FILE *fp = fopen(LOG_FILE, "a");
    if (!fp) return;
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    fprintf(fp, "[%02d:%02d:%04d]-[%02d:%02d:%02d]%s%s\n",
        t->tm_mday, t->tm_mon+1, t->tm_year+1900,
        t->tm_hour, t->tm_min, t->tm_sec, proc, status);
    fclose(fp);
}

int is_numeric(const char *s) {
    while (*s) if (!isdigit(*s++)) return 0;
    return 1;
}

int get_uid(const char *user) {
    struct passwd *p = getpwnam(user);
    return p ? p->pw_uid : -1;
}

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

void revert_mode(const char *user) {
    printf("[DEBUGMON] oke, dah aman '%s', lanjut lanjut\n", user);
    log_status(user, "RUNNING");
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("banh, debugmon tu makenya %s [list|daemon|stop|fail|revert] <user> loh, coba ulang\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "list") == 0) {
        list_processes(argv[2]);
    } else if (strcmp(argv[1], "daemon") == 0) {
        daemon_mode(argv[2]);
    } else if (strcmp(argv[1], "stop") == 0) {
        kill_processes(argv[2], "RUNNING");
    } else if (strcmp(argv[1], "fail") == 0) {
        kill_processes(argv[2], "FAILED");
    } else if (strcmp(argv[1], "revert") == 0) {
        revert_mode(argv[2]);
    } else {
        printf("perintah swalah, coba cek lagi banh~\n");
    }

    return 0;
}
