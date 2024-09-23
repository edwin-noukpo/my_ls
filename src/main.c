/*
** EPITECH PROJECT, 2024
** MY_NAVY
** File description:
** Function to count number of word in a function
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../include/utils.h"
#include "../include/struct.h"
#include "../include/robot.h"
#include "../include/all_linked_list/doubly_linked_list/d_list.h"
#include "../include/tree/tree.h"
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>


typedef struct my_ls_s {
    int ac;
    char **av;
    int index_av;
} my_ls_t;

typedef struct options_s {
    char flag;
    struct options_s *n;
} options_t;

typedef struct filepath_s {
    char *file_path;
    struct filepath_s *n;
} filepath_t;

typedef struct dirpath_s {
    char *dir_path;
    struct dirpath_s *n;
} dirpath_t;

typedef struct display_elements_s {
    char *file_type;
    char *file_permissions;
    int hard_link;
    char *file_owner;
    char *user_group;
    int file_size;
    char *date_and_time;
    char *file_name;
} display_elements_t;

typedef struct display_myls_s {
    char *dirpath;
    display_elements_t *elements;
    struct display_myls_s *n;
} display_myls_t;

void add_flags(options_t **list_opt, char flag)
{
    options_t *new = malloc(sizeof(options_t));
    options_t *temp = (options_t *)0x0;
    options_t *temp2 = (options_t *)0x0;

    *new = (options_t) {flag, (options_t *)0x0};
    if (!(*list_opt)) {
        (*list_opt) = new;
        return;
    }
    temp = (*list_opt);
    for (; temp; temp = temp->n)
        if (temp->flag == flag) {
            free(new);
            return;
        }
    temp2 = (*list_opt);
    for (; temp2->n; temp2 = temp2->n);
    temp2->n = new;
    return;
}

void add_filepath(filepath_t **list_file, char *file_path)
{
    filepath_t *new = malloc(sizeof(filepath_t));
    filepath_t *temp = (filepath_t *)0x0;
    filepath_t *temp2 = (filepath_t *)0x0;

    *new = (filepath_t) {file_path, (filepath_t *)0x0};
    if (!(*list_file)) {
        (*list_file) = new;
        return;
    }
    temp2 = (*list_file);
    for (; temp2->n; temp2 = temp2->n);
    temp2->n = new;
    return;
}

void add_dirpath(dirpath_t **list_dir, char *dir_path)
{
    dirpath_t *new = malloc(sizeof(dirpath_t));
    dirpath_t *temp = (dirpath_t *)0x0;
    dirpath_t *temp2 = (dirpath_t *)0x0;

    *new = (dirpath_t) {dir_path, (dirpath_t *)0x0};
    if (!(*list_dir)) {
        (*list_dir) = new;
        return;
    }
    temp2 = (*list_dir);
    for (; temp2->n; temp2 = temp2->n);
    temp2->n = new;
    return;
}

void disp_options(options_t *list_opt)
{
    options_t *temp = list_opt;

    for (; temp; temp = temp->n)
        printf("flags: %c\n", temp->flag);
    return;
}

void disp_filepath(filepath_t *filepath)
{
    filepath_t *temp = filepath;

    for (; temp; temp = temp->n)
        printf("filepath: %s\n", temp->file_path);
    return;
}

void disp_dirpath(dirpath_t *dirpath)
{
    dirpath_t *temp = dirpath;

    for (; temp; temp = temp->n)
        printf("dirpath: %s\n", temp->dir_path);
    return;
}

void free_options_s(options_t **list_opt)
{
    options_t *temp = (*list_opt);

    if (!(*list_opt))
        return;
    temp = (*list_opt);
    (*list_opt) = (*list_opt)->n;
    free_f(temp);
    return;
}

void free_dirpath_s(dirpath_t **list_dir)
{
    dirpath_t *temp = (*list_dir);

    if (!(*list_dir))
        return;
    temp = (*list_dir);
    (*list_dir) = (*list_dir)->n;
    free_f(temp);
    return;
}

void free_filepath_s(filepath_t **list_file)
{
    filepath_t *temp = (*list_file);

    if (!(*list_file))
        return;
    temp = (*list_file);
    (*list_file) = (*list_file)->n;
    free_f(temp);
    return;
}

// void free_options(options_t **list_opt)
// {
//     options_t *temp = (*list_opt);

//     for (; temp; temp = temp->n)
//         free_options_s(&temp);
// }

int main(int ac, char **av)
{
    my_ls_t *myls = malloc(sizeof(my_ls_t)); 
    options_t *list_opt = (options_t *)0x0;
    filepath_t *list_file = (filepath_t *)0x0;
    dirpath_t *list_dir = (dirpath_t *)0x0;
    *myls = (my_ls_t) {ac, av, 1};

    struct stat s;

    for(; myls->av[myls->index_av]; myls->index_av++) {
        if (*myls->av[myls->index_av] == '-' && my_strlen(myls->av[myls->index_av]) > 1)
            for(int x = 1; myls->av[myls->index_av][x]; x++)
                add_flags(&list_opt, myls->av[myls->index_av][x]);
        DIR *directory =  opendir(myls->av[myls->index_av]);
        if (!stat(myls->av[myls->index_av], &s) && !directory)
            add_filepath(&list_file, myls->av[myls->index_av]);
        if (directory) {
            add_dirpath(&list_dir, myls->av[myls->index_av]);
            closedir(directory);
        }
        if (*myls->av[myls->index_av] != '-' && stat(myls->av[myls->index_av], &s) == -1 && !directory) {
            err_mini_printf("error opening of: %s\n", myls->av[myls->index_av]);
        }
    }
    disp_filepath(list_file);
    // disp_options(list_opt);
    // disp_dirpath(list_dir);
    if (!list_dir) {
        dirpath_t *new = malloc(sizeof(dirpath_t));
        *new = (dirpath_t) {".", (dirpath_t *)0x0};
        list_dir = new; 
    }
    DIR *directory = (DIR *)0x0;
    struct dirent *dirent_directory;
    dirpath_t *temp = list_dir;
    for (; temp; temp = temp->n) {
        directory = opendir(temp->dir_path);
        int total = 0;
        for (dirent_directory = readdir(directory); dirent_directory; dirent_directory = readdir(directory)) {
            char *dir_tmp = my_strcat(temp->dir_path, "/");
            dir_tmp = my_strcat(dir_tmp, dirent_directory->d_name);
            int link = 1;
            /*
            total_int = ceil[
                (file1_physical_blocks_in_use * file1_physical_block_size / file1_ls_block_size) + 
                (file2_physical_blocks_in_use * file2_physical_block_size / file2_ls_block_size) + 
                ... + 
                (fileN_physical_blocks_in_use * fileN_physical_block_size / fileN_ls_block_size)
            ]
            */
            if (!stat(dir_tmp, &s)) {
                // mini_printf("File system block size: %d\nPhysical block size of file %d\n Physical Blocks in Use: %d\n\n", s.st_blocks, s.st_blksize, ((s.st_size/s.st_blksize) + 1) * 4);
                double block = ((double)s.st_size / 1024.0);
                // printf("%.5f-------%d\n", block , (int)block);
                // if (block > 4)
                //     block += 4;
                if (block > 4.0 && block > (int)block)
                    block += 1;
                // if (block > 0 && block < 4)
                //     block = 4;`
                if (s.st_size < 4096)
                    block = 0;
                char *file_type = "s";
                if (S_ISREG(s.st_mode))
                    file_type = "-";
                if (S_ISDIR(s.st_mode)) {
                    DIR *directory = opendir(dir_tmp);
                    struct dirent *dirent_directory;
                    struct stat s;
                    link = 0;
                    for (dirent_directory = readdir(directory); dirent_directory; dirent_directory = readdir(directory)) {
                        char *dir_tmp2 = my_strcat(dir_tmp, "/");
                        dir_tmp2 = my_strcat(dir_tmp2, dirent_directory->d_name);
                        dir_tmp2 = my_strcat(dir_tmp2, "/");
                        if (!stat(dir_tmp2, &s)) {
                            if (S_ISDIR(s.st_mode)) {
                                link++;
                            }
                        }
                        free_f(dir_tmp2);
                    }
                    closedir(directory);
                    file_type = "d";
                }
                if (S_ISLNK(s.st_mode))
                    file_type = "l";
                if (S_ISCHR(s.st_mode))
                    file_type = "c";
                if (S_ISBLK(s.st_mode))
                    file_type = "b";
                if (S_ISFIFO(s.st_mode))
                    file_type = "p";
                char *file_permission = (char *)0x0;
                file_permission = my_strcat((s.st_mode & S_IRUSR) ? "r" : "-", (s.st_mode & S_IWUSR) ? "w" : "-");
                file_permission = my_strcat(file_permission, (s.st_mode & S_IXUSR) ? "x" : "-");
                file_permission = my_strcat(file_permission, (s.st_mode & S_IRGRP) ? "r" : "-");
                file_permission = my_strcat(file_permission, (s.st_mode & S_IWGRP) ? "w" : "-");
                file_permission = my_strcat(file_permission, (s.st_mode & S_IXGRP) ? "x" : "-");
                file_permission = my_strcat(file_permission, (s.st_mode & S_IROTH) ? "r" : "-");
                file_permission = my_strcat(file_permission, (s.st_mode & S_IWOTH) ? "w" : "-");
                file_permission = my_strcat(file_permission, (s.st_mode & S_IXOTH) ? "x" : "-");
                struct passwd *uid;
                struct group *gid;
                uid = getpwuid(s.st_uid);
                gid = getgrgid(s.st_gid);
                char *date_time = ctime(&s.st_mtime) + 4;
                char **tab_date_time = split(date_time, ":\n");
                char *tmp_date_time = my_strcat(*tab_date_time, ":");
                tmp_date_time = my_strcat(tmp_date_time, tab_date_time[1]);
                free_2d_array(tab_date_time);
                total += (int)block;
                printf("%d\t%s%s %d\t%s %s\t%d\t%s %s\n", (int)block, file_type,
                file_permission,
                link, uid->pw_name,
                gid->gr_name, s.st_size,
                tmp_date_time,
                dirent_directory->d_name);
                free_f(file_permission);
                free_f(tmp_date_time);
                free_f(dir_tmp);
            } else {
                err_mini_printf("error opening of: %s\n", dirent_directory->d_name);
            }
        }
        // total = (total / 2);
        mini_printf("total: %d\n", total);
        closedir(directory);
    }
    free_f(myls);
    return 0;
}
