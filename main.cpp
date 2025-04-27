//
// Created by Phuc Le Vinh on 14/3/25.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SinhVien.h"

// Đọc file CSV và lưu vào mảng
void readCSV(const char *filename, SinhVien **svList, int &count) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Khong the mo file %s\n", filename);
        return;
    }

    // Đếm số dòng để cấp phát mảng
    int lines = 0;
    char buffer[2000];
    while (fgets(buffer, sizeof(buffer), file)) {
        lines++;
    }
    lines--; // Bỏ qua dòng tiêu đề

    // Cấp phát mảng SinhVien
    *svList = (SinhVien*)malloc(lines * sizeof(SinhVien));
    if (!*svList) {
        printf("Khong the cap phat bo nho\n");
        fclose(file);
        return;
    }

    // Đặt con trỏ file về đầu
    rewind(file);
    fgets(buffer, sizeof(buffer), file); // Bỏ qua dòng tiêu đề

    while (fgets(buffer, sizeof(buffer), file) && count < lines) {
        // Xóa ký tự xuống dòng
        buffer[strcspn(buffer, "\n")] = 0;

        // Tách chuỗi
        char* token = strtok(buffer, ",");
        if (token) strncpy((*svList)[count].MSSV, token, sizeof((*svList)[count].MSSV) - 1);

        token = strtok(NULL, ",");
        if (token) strncpy((*svList)[count].TenSV, token, sizeof((*svList)[count].TenSV) - 1);

        token = strtok(NULL, ",");
        if (token) strncpy((*svList)[count].Email, token, sizeof((*svList)[count].Email) - 1);

        token = strtok(NULL, ",");
        if (token) strncpy((*svList)[count].Khoa, token, sizeof((*svList)[count].Khoa) - 1);

        token = strtok(NULL, ",");
        if (token) strncpy((*svList)[count].KhoaHoc, token, sizeof((*svList)[count].KhoaHoc) - 1);

        token = strtok(NULL, ",");
        if (token) strncpy((*svList)[count].NgaySinh, token, sizeof((*svList)[count].NgaySinh) - 1);

        token = strtok(NULL, ",");
        if (token) strncpy((*svList)[count].HinhAnh, token, sizeof((*svList)[count].HinhAnh) - 1);

        token = strtok(NULL, ",");
        if (token) strncpy((*svList)[count].MoTaBanThan, token, sizeof((*svList)[count].MoTaBanThan) - 1);

        token = strtok(NULL, ",");
        if (token) strncpy((*svList)[count].SoThich1, token, sizeof((*svList)[count].SoThich1) - 1);

        token = strtok(NULL, ",");
        if (token) strncpy((*svList)[count].SoThich2, token ? token : "", sizeof((*svList)[count].SoThich2) - 1);

        count++;
    }

    fclose(file);
}


// Đọc file template HTML
char *readTemplate(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Không thể mở file template HTML\n");
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

// Thay thế các placeholder trong template HTML
void replaceAndSave(const char *templateHTML, SinhVien sv, const char *outputFile) {
    // Mở file để ghi HTML
    FILE *file = fopen(outputFile, "w");
    if (!file) {
        printf("Không thể tạo file HTML output\n");
        exit(1);
    }

    const int BUF_SIZE = 1024 * 20;
    char buffer[BUF_SIZE];
    char tempBuf[BUF_SIZE];
    // sao chép template vào buffer
    strncpy(buffer, templateHTML, BUF_SIZE - 1);
    buffer[BUF_SIZE - 1] = '\0';

    // Giá trị tương ứng với danh sách placeholder
    const char *templateValues[] = {sv.MSSV, sv.TenSV, sv.Khoa, sv.KhoaHoc, sv.NgaySinh, sv.HinhAnh, sv.MoTaBanThan, sv.SoThich1, sv.SoThich2, sv.Email};
    size_t count = sizeof(placeholders) / sizeof(placeholders[0]);

    // Thay thế từng placeholder
    for (size_t i = 0; i < count; ++i) {
        char *pos;
        while ((pos = strstr(buffer, placeholders[i]))) {
            size_t prefixLen = pos - buffer;
            memcpy(tempBuf, buffer, prefixLen);
            tempBuf[prefixLen] = '\0';
            strncat(tempBuf, templateValues[i], BUF_SIZE - strlen(tempBuf) - 1);
            strncat(tempBuf, pos + strlen(placeholders[i]), BUF_SIZE - strlen(tempBuf) - 1);
            strncpy(buffer, tempBuf, BUF_SIZE - 1);
            buffer[BUF_SIZE - 1] = '\0';
        }
    }

    // ghi buffer đã thay thế vào file
    fputs(buffer, file);
    fclose(file);
}

int main() {
    SinhVien *svList;
    int count;

    readCSV("sinhvien.csv", &svList, count);

    char *templateHTML = readTemplate("template.html");

    for (int i = 0; i < count; i++) {
        char filename[50];
        snprintf(filename, sizeof(filename), "output/%s.html", svList[i].MSSV);
        replaceAndSave(templateHTML, svList[i], filename);
    }

    free(templateHTML);
    printf("Xuất HTML thành công!\n");

    return 0;
}
