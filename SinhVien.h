#define NUM_FIELDS 10

typedef struct {
    char MSSV[11];
    char TenSV[31];
    char Email[50];
    char Khoa[31];
    char KhoaHoc[4]; // Khoá học (VD: 2020)
    char NgaySinh[11];
    char HinhAnh[200];
    char MoTaBanThan[1000];
    char SoThich1[100];
    char SoThich2[100];
} SinhVien;

const char *placeholders[] = {"<MSSV>", "<TenSV>", "<Khoa>", "<KhoaHoc>", "<NgaySinh>", "<HinhAnh>", "<MoTaBanThan>", "<SoThich1>", "<SoThich2>", "<Email>"};