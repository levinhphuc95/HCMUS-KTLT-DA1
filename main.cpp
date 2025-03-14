//
// Created by Phuc Le Vinh on 14/3/25.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024
#define MAX_SV   100

typedef struct {
    char MSSV[20];
    char TenSV[100];
    char Khoa[50];
    char KhoaHoc[10]; // Khoá học (VD: 2020)
} SinhVien;

void readCSV(const char *filename, SinhVien svList[], int *count) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Không thể mở file CSV");
        exit(1);
    }

    char line[MAX_LINE];
    *count = 0;

    fgets(line, sizeof(line), file); // Bỏ qua dòng tiêu đề CSV

    while (fgets(line, sizeof(line), file) && *count < MAX_SV) {
        SinhVien *sv = &svList[*count];
        sscanf(line, "%[^,],%[^,],%[^,],%s", sv->MSSV, sv->TenSV, sv->Khoa, sv->KhoaHoc);
        (*count)++;
    }

    fclose(file);
}

char *readTemplate(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Không thể mở file template HTML");
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char *buffer = (char *)malloc(size + 1);
    fread(buffer, 1, size, file);
    buffer[size] = '\0';

    fclose(file);
    return buffer;
}

void replaceAndSave(const char *templateHTML, SinhVien sv, const char *outputFile) {
    FILE *file = fopen(outputFile, "w");
    if (!file) {
        perror("Không thể tạo file HTML output");
        exit(1);
    }

    char buffer[MAX_LINE * 10]; // Giữ nội dung đã thay thế
    strcpy(buffer, templateHTML);

    // Thay thế các placeholder
    char temp[MAX_LINE];
    snprintf(temp, sizeof(temp), buffer, sv.MSSV, sv.TenSV, sv.Khoa, sv.KhoaHoc);

    fputs(temp, file);
    fclose(file);
}

int main() {
    SinhVien svList[MAX_SV];
    int count;

    readCSV("../sinhvien.csv", svList, &count);

    char *templateHTML = readTemplate("../template.html");

    for (int i = 0; i < count; i++) {
        char filename[50];
        snprintf(filename, sizeof(filename), "output_%s.html", svList[i].MSSV);
        replaceAndSave(templateHTML, svList[i], filename);
    }

    free(templateHTML);
    printf("Xuất HTML thành công!\n");

    return 0;
}
