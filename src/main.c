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

typedef struct tmp_elements_s {
    char *type;
    char *rwxrwxrwx;
    int hard_link;
    char *uid;
    char *gid;
    int size;
    char *time;
    char *name;
} tmp_elements_t;

typedef struct outputfile_s {
    char *type;
    char *rwxrwxrwx;
    int hard_link;
    char *uid;
    char *gid;
    int size;
    char *time;
    char *name;
    struct outputfile_s *n;
} outputfile_t;

typedef struct outputdir_s {
    char *type;
    char *rwxrwxrwx;
    int hard_link;
    char *uid;
    char *gid;
    int size;
    char *time;
    char *name;
    struct outputdir_s *n;
} outputdir_t;

typedef struct dir_elem_s {
    char *dirpath;
    int *data;
    outputdir_t *output;
    struct dir_elem_s *n;
} dir_elem_t;

void add_outputfile(outputfile_t **outputfile, tmp_elements_t elem)
{
    outputfile_t *new = malloc(sizeof(outputfile_t));
    outputfile_t *temp = (outputfile_t *)0x0;

    *new = (outputfile_t) {elem.type, elem.rwxrwxrwx, elem.hard_link, elem.uid, elem.gid, elem.size, elem.time, elem.name, (outputfile_t *)0x0};
    if (!(*outputfile) || strcmp((*outputfile)->name, elem.name) > 0) {
        new->n = (*outputfile);
        (*outputfile) = new;
        return;
    }
    temp = (*outputfile);
    for (; temp->n && strcmp(temp->n->name, elem.name) < 0; temp = temp->n);
    new->n = temp->n;
    temp->n = new;
    return;
}

void add_outputdir(outputdir_t **outputdir, tmp_elements_t elem)
{
    outputdir_t *new = malloc(sizeof(outputdir_t));
    outputdir_t *temp = (outputdir_t *)0x0;

    *new = (outputdir_t) {elem.type, elem.rwxrwxrwx, elem.hard_link, elem.uid, elem.gid, elem.size, elem.time, elem.name, (outputdir_t *)0x0};
    if (!(*outputdir) || strcmp((*outputdir)->name, elem.name) > 0) {
        new->n = (*outputdir);
        (*outputdir) = new;
        return;
    }
    temp = (*outputdir);
    for (; temp->n && strcmp(temp->n->name, elem.name) < 0; temp = temp->n);
    new->n = temp->n;
    temp->n = new;
    return;
}

void add_dir_elem(dir_elem_t **dir_elem, char *dirpath, int *data, outputdir_t *output)
{
    dir_elem_t *new = malloc(sizeof(dir_elem_t));
    dir_elem_t *temp = (dir_elem_t *)0x0;

    *new = (dir_elem_t) {dirpath, data, output, (dir_elem_t *)0x0};
    if (!(*dir_elem) || strcmp((*dir_elem)->dirpath, dirpath) > 0) {
        new->n = (*dir_elem);
        (*dir_elem) = new;
        return;
    }
    temp = (*dir_elem);
    for (; temp->n && strcmp(temp->n->dirpath, dirpath) < 0; temp = temp->n);
    new->n = temp->n;
    temp->n = new;
    return;
}

void disp_file(outputfile_t *outputfile)
{
    outputfile_t *temp = outputfile;
    for (; temp; temp = temp->n)
        mini_printf("%s%s %d\t%s %s\t%d\t%s %s\n", temp->type, temp->rwxrwxrwx, temp->hard_link, temp->uid, temp->gid, temp->size, temp->time, temp->name);
    return;
}

void disp_dir(outputdir_t *outputdir)
{
    outputdir_t *temp = outputdir;
    for (; temp; temp = temp->n)
        mini_printf("%s%s %d\t%s %s\t%d\t%s %s\n", temp->type, temp->rwxrwxrwx, temp->hard_link, temp->uid, temp->gid, temp->size, temp->time, temp->name);
    return;
}
void display(dir_elem_t *dir_elem)
{
    dir_elem_t *temp = dir_elem;

    for (; temp; temp = temp->n) {
        mini_printf("%s:\n", temp->dirpath);
        mini_printf("total: %d\n", temp->data[2]);
        disp_dir(temp->output);
        if (temp->n)
            mini_printf("\n", temp->dirpath);
    }
    return;
}

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
    if (!list_dir && !list_file) {
        dirpath_t *new = malloc(sizeof(dirpath_t));
        *new = (dirpath_t) {".", (dirpath_t *)0x0};
        list_dir = new; 
    }
    outputfile_t *outputfile = (outputfile_t *)0x0;
    filepath_t *temp = list_file;
    for (; temp; temp = temp->n) {
        if (!lstat(temp->file_path, &s)) {
            char *file_type = "s";
            if (S_ISREG(s.st_mode))
                file_type = "-";
            if (S_ISLNK(s.st_mode))
                file_type = "l";
            if (S_ISCHR(s.st_mode))
                file_type = "c";
            if (S_ISBLK(s.st_mode))
                file_type = "b";
            if (S_ISFIFO(s.st_mode))
                file_type = "p";
            char *file_permission = my_strcat((s.st_mode & S_IRUSR) ? "r" : "-", (s.st_mode & S_IWUSR) ? "w" : "-");
            file_permission = my_strcat(file_permission, (s.st_mode & S_IXUSR) ? "x" : "-");
            file_permission = my_strcat(file_permission, (s.st_mode & S_IRGRP) ? "r" : "-");
            file_permission = my_strcat(file_permission, (s.st_mode & S_IWGRP) ? "w" : "-");
            file_permission = my_strcat(file_permission, (s.st_mode & S_IXGRP) ? "x" : "-");
            file_permission = my_strcat(file_permission, (s.st_mode & S_IROTH) ? "r" : "-");
            file_permission = my_strcat(file_permission, (s.st_mode & S_IWOTH) ? "w" : "-");
            file_permission = my_strcat(file_permission, (s.st_mode & S_IXOTH) ? "x" : "-");
            struct passwd *uid = getpwuid(s.st_uid);
            struct group *gid = getgrgid(s.st_gid);
            char *date_time = ctime(&s.st_mtime) + 4;
            char **tab_date_time = split(date_time, ":\n");
            char *tmp_date_time = my_strcat(*tab_date_time, ":");
            tmp_date_time = my_strcat(tmp_date_time, tab_date_time[1]);
            free_2d_array(tab_date_time);;
            tmp_elements_t elem = (tmp_elements_t) {file_type, file_permission, 1,uid->pw_name, gid->gr_name,s.st_size, tmp_date_time, temp->file_path};
            add_outputfile(&outputfile, elem);
        } else
            err_mini_printf("error opening of: %s\n", temp->file_path);
    }
    DIR *directory = (DIR *)0x0;
    struct dirent *dirent_directory;
    dirpath_t *temp2 = list_dir;
    dir_elem_t *dir_elem = (dir_elem_t *)0x0;
    for (; temp2; temp2 = temp2->n) {
        int *data = malloc(sizeof(int) * 3);
        outputdir_t *output = (outputdir_t *)0x0;
        directory = opendir(temp2->dir_path);
        int total = 0;
        for (dirent_directory = readdir(directory); dirent_directory; dirent_directory = readdir(directory)) {
            char *dir_tmp = my_strcat(temp2->dir_path, "/");
            dir_tmp = my_strcat(dir_tmp, dirent_directory->d_name);
            int link = 1;
            if (!lstat(dir_tmp, &s)) {
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
                        if (!lstat(dir_tmp2, &s)) {
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
                char *file_permission = my_strcat((s.st_mode & S_IRUSR) ? "r" : "-", (s.st_mode & S_IWUSR) ? "w" : "-");
                file_permission = my_strcat(file_permission, (s.st_mode & S_IXUSR) ? "x" : "-");
                file_permission = my_strcat(file_permission, (s.st_mode & S_IRGRP) ? "r" : "-");
                file_permission = my_strcat(file_permission, (s.st_mode & S_IWGRP) ? "w" : "-");
                file_permission = my_strcat(file_permission, (s.st_mode & S_IXGRP) ? "x" : "-");
                file_permission = my_strcat(file_permission, (s.st_mode & S_IROTH) ? "r" : "-");
                file_permission = my_strcat(file_permission, (s.st_mode & S_IWOTH) ? "w" : "-");
                file_permission = my_strcat(file_permission, (s.st_mode & S_IXOTH) ? "x" : "-");
                struct passwd *uid = getpwuid(s.st_uid);
                struct group *gid = getgrgid(s.st_gid);
                char *date_time = ctime(&s.st_mtime) + 4;
                char **tab_date_time = split(date_time, ":\n");
                char *tmp_date_time = my_strcat(*tab_date_time, ":");
                tmp_date_time = my_strcat(tmp_date_time, tab_date_time[1]);
                free_2d_array(tab_date_time);
                total += (int)(s.st_blocks / 2);
                if (!my_strcmp(dirent_directory->d_name, "."))
                    data[0] = s.st_blocks / 2;
                if (!my_strcmp(dirent_directory->d_name, ".."))
                    data[1] = s.st_blocks / 2;
                tmp_elements_t elem = (tmp_elements_t) {file_type, file_permission, link ,uid->pw_name, gid->gr_name,s.st_size, tmp_date_time, dirent_directory->d_name};
                add_outputdir(&output, elem);
                free_f(dir_tmp);
            } else
                err_mini_printf("error opening of: %s\n", dirent_directory->d_name);
        }
        data[2] = total;
        add_dir_elem(&dir_elem, temp2->dir_path, data, output);
    }
    disp_file(outputfile);
    display(dir_elem);
    free_f(myls);
    return 0;
}
