// 22000217 Na,Yeawon
void flag_f(char *s, char *d); // function for -f option
int flag_m (int argc, char *argv[]); // function for -m option
void flag_d (char *s, char *d, int v); // function for -d option
void flag_v (char *s, char *d); // function for -v option

int main(int argc, char *argv[]) {
    int opt;
    int f = 0, m = 0, d = 0, v = 0;

    while((opt = getopt(argc, argv, "fmdv")) != -1) {
        switch (opt) {
            case 'f': f = 1; break;
            case 'm': m = 1; break;
            case 'd': d = 1; break;
            case 'v': v = 1; break;
            default: printf("Unknown flag : %c \n", optopt);
        }
    }
    
    if (f) {
        flag_f(argv[2], argv[3]);
        if (v) flag_v(argv[2], argv[3]);
    }
    if (m) {
        int result = flag_m(argc, argv);
        if (result && v) {
            for (int i=2; i<argc-1; i++) {
                char* d = malloc(strlen(argv[argc-1]));
                strcpy(d, argv[argc-1]);
                strcat(d, argv[i]);
                flag_v(argv[i], d);
            }
        }
    }
    if (d) {
        flag_d(argv[2], argv[3], v);
    }
    return 0;
}

// -f option
/*
1. Check if the source file exists.
2. Check the permissions of the source file.
3. Copy the source file to the destination file.
*/
void flag_f(char *s, char *d) {
    struct stat sb;
    int src, dst;
    char buf[1024];
    int n;
    if (s == NULL || d == NULL) { perror("Usage: copyfile -f file1 file2"); return;}
    if (stat(s, &sb) == -1) { perror("stat"); return; }

    src = open(s, O_RDONLY);
    dst = open(d, O_WRONLY | O_CREAT | O_TRUNC, sb.st_mode);

    while ((n = read(src, buf, sizeof(buf))) > 0) {
        write(dst, buf, n);
    }

    close(src);
    close(dst);
}

// -m option
/* 
1. If the target directory does not exist, return.
2. Else, for given files, copy them to the directory by using -f option.
*/
int flag_m (int argc, char *argv[]) {
    DIR *dp;
    if((dp = opendir(argv[argc-1])) == NULL) { perror("opendir"); return 0; }
    argv[argc-1] = strcat(argv[argc-1], "/");
    for (int i=2; i<argc-1; i++) {
        char* d = malloc(strlen(argv[argc-1] + 1024));
        strcpy(d, argv[argc-1]);
        flag_f(argv[i], strcat(d, argv[i]));
        free(d);
    }
    return 1;
}

// -d option
/*
1. Open directory. If the source directory does not exist, return.
2. For each file in the directory, copy it to the destination by using -f option.
cf. To reduce the iteration, this function calls flag_v() function directly.
*/
void flag_d (char *s, char *d, int v) {
    DIR *dp;
    struct dirent *dir = NULL;
    if((dp = opendir(s)) == NULL) { return; }
    struct stat sb;
    mkdir(d, sb.st_mode);
    d = strcat(d, "/");
    while ((dir = readdir(dp)) != NULL) {
        if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) continue;
        else if (dir->d_type == DT_DIR) { // if it's a directory
            char* d2 = malloc(strlen(d) + 1024);
            strcpy(d2, d);
            strcat(d2, dir->d_name);
            char* s2 = malloc(strlen(s) + 1024);
            strcpy(s2, s); strcat(s2, "/");
            flag_d(strcat(s2, dir->d_name), d2, v);
            free(d2); free(s2);
        }
        else {
            char* src = malloc(strlen(s) + 1024); strcpy(src, s);
            char* dst = malloc(strlen(d) + 1024); strcpy(dst, d);
            flag_f(dir->d_name, strcat(dst, dir->d_name));
            if (v) {
                strcat(src, "/");
                strcat(src, dir->d_name);
                flag_v(src, dst);
            }
            free(src); free(dst);
        }
    }
}

// -v option
/*
1. Print the source and destination file names.
*/
void flag_v (char *s, char *d) {
    printf("Copy File : %s -> %s\n", s, d);
}