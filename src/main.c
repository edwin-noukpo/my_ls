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
    struct outputfile_s *p;
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
    struct outputdir_s *p;
    struct outputdir_s *n;
} outputdir_t;

typedef struct dir_elem_s {
    char *dirpath;
    int *data;
    outputdir_t *output;
    struct dir_elem_s *p;
    struct dir_elem_s *n;
} dir_elem_t;

int my_datecmp(char *strdate1, char *strdate2)
{
    char *calendar[] = {"Jan", "Fev", "Mar", "Apr", "May",
    "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec", (char *)0x0};
    char **tab_date1 = split(strdate1, " ");
    char **tab_date2 = split(strdate2, " ");

    int month1 = 0;
    for(; my_strcmp(tab_date1[0], calendar[month1]); month1++);
    char *date_time1 = my_strcat(tab_date1[1], tab_date1[2]);
    int month2 = 0;
    for(; my_strcmp(tab_date1[0], calendar[month2]); month2++);
    char *date_time2 = my_strcat(tab_date2[1], tab_date2[2]);

    int diff = 0;
    if (month1 == month2)
        diff = my_strcmp(date_time1, date_time2);
    if (month1 < month2)
        diff = -1;
    if (month1 > month2)
        diff = 1;
    free_2d_array(tab_date1);
    free_2d_array(tab_date2);
    return diff;
}

void add_timeoutputfile(outputfile_t **timeoutputfile, tmp_elements_t elem)
{
    outputfile_t *new = malloc(sizeof(outputfile_t));
    outputfile_t *temp = (outputfile_t *)0x0;

    *new = (outputfile_t) {elem.type, elem.rwxrwxrwx, elem.hard_link, elem.uid, elem.gid, elem.size, elem.time, elem.name, (outputfile_t *)0x0, (outputfile_t *)0x0};
    if (!(*timeoutputfile) || my_datecmp((*timeoutputfile)->time, elem.time) < 0) {
        new->n = (*timeoutputfile);
        if (*timeoutputfile)
            (*timeoutputfile)->p = new;
        (*timeoutputfile) = new;
        return;
    }
    temp = (*timeoutputfile);
    for (; temp->n && my_datecmp(temp->n->time, elem.time) > 0; temp = temp->n);
    if (!temp->n) {
        temp->n = new;
        new->p = temp;
        new->n = (outputfile_t *)0x0;
        return;
    }
    new->n = temp->n;
    new->p = temp;
    if (temp->n)
        temp->n->p = new;
    temp->n = new;
    return;
}

void add_timeoutputdir(outputdir_t **timeoutputdir, tmp_elements_t elem)
{
    outputdir_t *new = malloc(sizeof(outputdir_t));
    outputdir_t *temp = (outputdir_t *)0x0;

    *new = (outputdir_t) {elem.type, elem.rwxrwxrwx, elem.hard_link, elem.uid, elem.gid, elem.size, elem.time, elem.name, (outputdir_t *)0x0, (outputdir_t *)0x0};
    if (!(*timeoutputdir) || my_datecmp((*timeoutputdir)->time, elem.time) < 0) {
        new->n = (*timeoutputdir);
        if (*timeoutputdir)
            (*timeoutputdir)->p = new;
        (*timeoutputdir) = new;
        return;
    }
    temp = (*timeoutputdir);
    for (; temp->n && my_datecmp(temp->n->time, elem.time) > 0; temp = temp->n);
    if (!temp->n) {
        temp->n = new;
        new->p = temp;
        new->n = (outputdir_t *)0x0;
        return;
    }
    new->n = temp->n;
    new->p = temp;
    if (temp->n)
        temp->n->p = new;
    temp->n = new;
    return;
}

void add_outputfile(outputfile_t **outputfile, tmp_elements_t elem)
{
    outputfile_t *new = malloc(sizeof(outputfile_t));
    outputfile_t *temp = (outputfile_t *)0x0;

    *new = (outputfile_t) {elem.type, elem.rwxrwxrwx, elem.hard_link, elem.uid, elem.gid, elem.size, elem.time, elem.name, (outputfile_t *)0x0, (outputfile_t *)0x0};
    if (!(*outputfile) || my_strcmp((*outputfile)->name, elem.name) > 0) {
        new->n = (*outputfile);
        if (*outputfile)
            (*outputfile)->p = new;
        (*outputfile) = new;
        return;
    }
    temp = (*outputfile);
    for (; temp->n && my_strcmp(temp->n->name, elem.name) < 0; temp = temp->n);
    if (!temp->n) {
        temp->n = new;
        new->p = temp;
        new->n = (outputfile_t *)0x0;
        return;
    }
    new->n = temp->n;
    new->p = temp;
    if (temp->n)
        temp->n->p = new;
    temp->n = new;
    return;
}

void add_outputdir(outputdir_t **outputdir, tmp_elements_t elem)
{
    outputdir_t *new = malloc(sizeof(outputdir_t));
    outputdir_t *temp = (outputdir_t *)0x0;

    *new = (outputdir_t) {elem.type, elem.rwxrwxrwx, elem.hard_link, elem.uid, elem.gid, elem.size, elem.time, elem.name, (outputdir_t *)0x0, (outputdir_t *)0x0};
    if (!(*outputdir) || my_strcmp((*outputdir)->name, elem.name) > 0) {
        new->n = (*outputdir);
        if (*outputdir)
            (*outputdir)->p = new;
        (*outputdir) = new;
        return;
    }
    temp = (*outputdir);
    for (; temp->n && my_strcmp(temp->n->name, elem.name) < 0; temp = temp->n);
    if (!temp->n) {
        temp->n = new;
        new->p = temp;
        new->n = (outputdir_t *)0x0;
        return;
    }
    new->n = temp->n;
    new->p = temp;
    if (temp->n)
        temp->n->p = new;
    temp->n = new;
    return;
}

void add_dir_elem(dir_elem_t **dir_elem, char *dirpath, int *data, outputdir_t *output)
{
    dir_elem_t *new = malloc(sizeof(dir_elem_t));
    dir_elem_t *temp = (dir_elem_t *)0x0;

    *new = (dir_elem_t) {dirpath, data, output, (dir_elem_t *)0x0, (dir_elem_t *)0x0};
    if (!(*dir_elem) || my_strcmp((*dir_elem)->dirpath, dirpath) > 0) {
        new->n = (*dir_elem);
        if (*dir_elem)
            (*dir_elem)->p = new;
        (*dir_elem) = new;
        return;
    }
    temp = (*dir_elem);
    for (; temp->n && my_strcmp(temp->n->dirpath, dirpath) < 0; temp = temp->n);
    if (!temp->n) {
        temp->n = new;
        new->p = temp;
        new->n = (dir_elem_t *)0x0;
        return;
    }
    new->n = temp->n;
    new->p = temp;
    if (temp->n)
        temp->n->p = new;
    temp->n = new;
    return;
}

void disp_file(outputfile_t *outputfile)
{
    outputfile_t *temp = outputfile;

    if (!temp)
        return;
    for (; temp; temp = temp->n)
        mini_printf("%s%s %d\t%s %s\t%d\t%s %s\n", temp->type, temp->rwxrwxrwx, temp->hard_link, temp->uid, temp->gid, temp->size, temp->time, temp->name);
    return;
}

void time_disp_file(outputfile_t *outputfile)
{

    if (!outputfile)
        return;
    outputfile_t *temp2 = outputfile;
    outputfile_t *timeoutputfile = (outputfile_t *)0x0;
    for (; temp2; temp2 = temp2->n) {
        tmp_elements_t elem = (tmp_elements_t) {temp2->type, temp2->rwxrwxrwx, temp2->hard_link ,temp2->uid, temp2->gid, temp2->size, temp2->time, temp2->name};
        add_timeoutputfile(&timeoutputfile, elem);
    }
    outputfile_t *temp = timeoutputfile;
    for (; temp; temp = temp->n)
        mini_printf("%s%s %d\t%s %s\t%d\t%s %s\n", temp->type, temp->rwxrwxrwx, temp->hard_link, temp->uid, temp->gid, temp->size, temp->time, temp->name);
    return;
}

void rev_time_disp_file(outputfile_t *outputfile)
{

    if (!outputfile)
        return;
    outputfile_t *temp2 = outputfile;
    outputfile_t *timeoutputfile = (outputfile_t *)0x0;
    for (; temp2; temp2 = temp2->n) {
        tmp_elements_t elem = (tmp_elements_t) {temp2->type, temp2->rwxrwxrwx, temp2->hard_link ,temp2->uid, temp2->gid, temp2->size, temp2->time, temp2->name};
        add_timeoutputfile(&timeoutputfile, elem);
    }
    outputfile_t *temp = timeoutputfile;
    for (; temp->n; temp = temp->n);
    for (; temp; temp = temp->p)
        mini_printf("%s%s %d\t%s %s\t%d\t%s %s\n", temp->type, temp->rwxrwxrwx, temp->hard_link, temp->uid, temp->gid, temp->size, temp->time, temp->name);
    return;
}

void rev_disp_file(outputfile_t *outputfile)
{
    outputfile_t *temp = outputfile;

    if (!temp)
        return;
    for (; temp->n; temp = temp->n);
    for (; temp; temp = temp->p)
        mini_printf("%s%s %d\t%s %s\t%d\t%s %s\n", temp->type, temp->rwxrwxrwx, temp->hard_link, temp->uid, temp->gid, temp->size, temp->time, temp->name);
    return;
}

void disp_dir(outputdir_t *outputdir)
{
    outputdir_t *temp = outputdir;

    if (!temp)
        return;
    for (; temp; temp = temp->n)
        mini_printf("%s%s %d\t%s %s\t%d\t%s %s\n", temp->type, temp->rwxrwxrwx, temp->hard_link, temp->uid, temp->gid, temp->size, temp->time, temp->name);
    return;
}

void time_display(dir_elem_t *dir_elem)
{
    dir_elem_t *temp = dir_elem;

    if (!temp)
        return;
    for (; temp; temp = temp->n) {
        mini_printf("%s:\n", temp->dirpath);
        mini_printf("total: %d\n", temp->data[2]);
        outputdir_t *timeoutdir = (outputdir_t *)0x0;
        outputdir_t *temp2 = temp->output;
        for (; temp2; temp2 = temp2->n) {
            tmp_elements_t elem = (tmp_elements_t) {temp2->type, temp2->rwxrwxrwx, temp2->hard_link ,temp2->uid, temp2->gid, temp2->size, temp2->time, temp2->name};
            add_timeoutputdir(&timeoutdir, elem);
        }
        disp_dir(timeoutdir);
        if (temp->n)
            mini_printf("\n", temp->dirpath);
    }
    return;
}

void display(dir_elem_t *dir_elem)
{
    dir_elem_t *temp = dir_elem;

    if (!temp)
        return;
    for (; temp; temp = temp->n) {
        mini_printf("%s:\n", temp->dirpath);
        mini_printf("total: %d\n", temp->data[2]);
        disp_dir(temp->output);
        if (temp->n)
            mini_printf("\n", temp->dirpath);
    }
    return;
}

void rev_disp_dir(outputdir_t *outputdir)
{
    outputdir_t *temp = outputdir;

    if (!temp)
        return;
    for (; temp->n; temp = temp->n);
    for (; temp; temp = temp->p)
        mini_printf("%s%s %d\t%s %s\t%d\t%s %s\n", temp->type, temp->rwxrwxrwx, temp->hard_link, temp->uid, temp->gid, temp->size, temp->time, temp->name);
    return;
}

void rev_display(dir_elem_t *dir_elem)
{
    dir_elem_t *temp = dir_elem;

    if (!temp)
        return;
    for (; temp->n; temp = temp->n);
    for (; temp; temp = temp->p) {
        mini_printf("%s:\n", temp->dirpath);
        mini_printf("total: %d\n", temp->data[2]);
        rev_disp_dir(temp->output);
        if (temp->p)
            mini_printf("\n", temp->dirpath);
    }
    return;
}

void rev_time_display(dir_elem_t *dir_elem)
{
    dir_elem_t *temp = dir_elem;

    if (!temp)
        return;
    for (; temp; temp = temp->n) {
        mini_printf("%s:\n", temp->dirpath);
        mini_printf("total: %d\n", temp->data[2]);
        outputdir_t *timeoutdir = (outputdir_t *)0x0;
        outputdir_t *temp2 = temp->output;
        for (; temp2; temp2 = temp2->n) {
            tmp_elements_t elem = (tmp_elements_t) {temp2->type, temp2->rwxrwxrwx, temp2->hard_link ,temp2->uid, temp2->gid, temp2->size, temp2->time, temp2->name};
            add_timeoutputdir(&timeoutdir, elem);
        }
        rev_disp_dir(timeoutdir);
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

    if (!temp)
        return;
    for (; temp; temp = temp->n)
        printf("flags: %c\n", temp->flag);
    return;
}

void disp_filepath(filepath_t *filepath)
{
    filepath_t *temp = filepath;

    if (!temp)
        return;
    for (; temp; temp = temp->n)
        printf("filepath: %s\n", temp->file_path);
    return;
}

void disp_dirpath(dirpath_t *dirpath)
{
    dirpath_t *temp = dirpath;

    if (!temp)
        return;
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
            char *rwxrwxrwx = malloc(sizeof(char) * 10); 
            rwxrwxrwx[0] = (s.st_mode & S_IRUSR) ? 'r' : '-';
            rwxrwxrwx[1] = (s.st_mode & S_IWUSR) ? 'w' : '-';
            rwxrwxrwx[2] = (s.st_mode & S_IXUSR) ? 'x' : '-';
            rwxrwxrwx[3] = (s.st_mode & S_IRGRP) ? 'r' : '-';
            rwxrwxrwx[4] = (s.st_mode & S_IWGRP) ? 'w' : '-';
            rwxrwxrwx[5] = (s.st_mode & S_IXGRP) ? 'x' : '-';
            rwxrwxrwx[6] = (s.st_mode & S_IROTH) ? 'r' : '-';
            rwxrwxrwx[7] = (s.st_mode & S_IWOTH) ? 'w' : '-';
            rwxrwxrwx[8] = (s.st_mode & S_IXOTH) ? 'x' : '-';
            rwxrwxrwx[9] = 0;
            struct passwd *uid = getpwuid(s.st_uid);
            struct group *gid = getgrgid(s.st_gid);
            char *date_time = ctime(&s.st_mtime) + 4;
            char **tab_date_time = split(date_time, ":\n");
            char *tmp_date_time = my_strcat(*tab_date_time, ":");
            tmp_date_time = my_strcat(tmp_date_time, tab_date_time[1]);
            free_2d_array(tab_date_time);
            tmp_elements_t elem = (tmp_elements_t) {file_type, rwxrwxrwx, 1,uid->pw_name, gid->gr_name,s.st_size, tmp_date_time, temp->file_path};
            add_outputfile(&outputfile, elem);
        } else
            err_mini_printf("error opening of: %s\n", temp->file_path);
    }
    dirpath_t *temp2 = list_dir;
    dir_elem_t *dir_elem = (dir_elem_t *)0x0;
    for (; temp2; temp2 = temp2->n) {
        DIR *directory = (DIR *)0x0;
        struct dirent *dirent_directory;
        int *data = malloc(sizeof(int) * 3);
        outputdir_t *output = (outputdir_t *)0x0;
        directory = opendir(temp2->dir_path);
        int total = 0;
        for (dirent_directory = readdir(directory); dirent_directory; dirent_directory = readdir(directory)) {
            char *dir_tmp = my_strcat(temp2->dir_path, "/");
            dir_tmp = my_strcat(dir_tmp, dirent_directory->d_name);
            int link = 1;
            char *d_name = my_strdup(dirent_directory->d_name);
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
                if (S_ISLNK(s.st_mode)) {
                    char *buffer = malloc(sizeof(char) * 256);
                    ssize_t link_len = readlink(dir_tmp, buffer, sizeof(char) * 256);
                    buffer[link_len] = 0;
                    d_name = my_strcat(d_name, " -> ");
                    d_name = my_strcat(d_name, buffer);
                    free_f(buffer);
                    file_type = "l";
                }
                if (S_ISCHR(s.st_mode))
                    file_type = "c";
                if (S_ISBLK(s.st_mode))
                    file_type = "b";
                if (S_ISFIFO(s.st_mode))
                    file_type = "p";
                char *rwxrwxrwx = malloc(sizeof(char) * 10);
                rwxrwxrwx[0] = (s.st_mode & S_IRUSR) ? 'r' : '-';
                rwxrwxrwx[1] = (s.st_mode & S_IWUSR) ? 'w' : '-';
                rwxrwxrwx[2] = (s.st_mode & S_IXUSR) ? 'x' : '-';
                rwxrwxrwx[3] = (s.st_mode & S_IRGRP) ? 'r' : '-';
                rwxrwxrwx[4] = (s.st_mode & S_IWGRP) ? 'w' : '-';
                rwxrwxrwx[5] = (s.st_mode & S_IXGRP) ? 'x' : '-';
                rwxrwxrwx[6] = (s.st_mode & S_IROTH) ? 'r' : '-';
                rwxrwxrwx[7] = (s.st_mode & S_IWOTH) ? 'w' : '-';
                rwxrwxrwx[8] = (s.st_mode & S_IXOTH) ? 'x' : '-';
                rwxrwxrwx[9] = 0;
                struct passwd *uid = getpwuid(s.st_uid);
                struct group *gid = getgrgid(s.st_gid);
                char *date_time = ctime(&s.st_mtime) + 4;
                char **tab_date_time = split(date_time, ":\n");
                char *tmp_date_time = my_strcat(*tab_date_time, ":");
                tmp_date_time = my_strcat(tmp_date_time, tab_date_time[1]);
                free_2d_array(tab_date_time);
                total += (int)(s.st_blocks / 2);
                if (!my_strcmp(dirent_directory->d_name, "."))
                    data[0] = (int)(s.st_blocks / 2);
                if (!my_strcmp(dirent_directory->d_name, ".."))
                    data[1] = (int)(s.st_blocks / 2);
                tmp_elements_t elem = (tmp_elements_t) {file_type, rwxrwxrwx, link ,uid->pw_name, gid->gr_name,s.st_size, tmp_date_time, d_name};
                add_outputdir(&output, elem);
                free_f(dir_tmp);
            } else
                err_mini_printf("error opening of: %s\n", dirent_directory->d_name);
        }
        data[2] = total;
        add_dir_elem(&dir_elem, temp2->dir_path, data, output);
        closedir(directory);
    }
    // disp_options(list_opt);
    // disp_filepath(list_file);
    // disp_dirpath(list_dir);
    disp_file(outputfile);
    // rev_disp_file(outputfile);
    // time_disp_file(outputfile);
    // rev_time_disp_file(outputfile);
    display(dir_elem);
    // rev_display(dir_elem);
    // time_display(dir_elem);
    // rev_time_display(dir_elem);
    free_f(myls);
    return 0;
}