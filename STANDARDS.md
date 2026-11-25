# STANDARDS.md: Quy chuẩn chung của nhóm (Đề 5: Giải thuật đệ quy và phân tích QuickSort)

Mục đích: ba người làm độc lập nhưng số liệu và biểu đồ ghép lại khớp 100%.
Quy tắc: không đổi file này khi chưa báo cả nhóm. Mỗi người đọc xong và nhắn **OK** vào nhóm chat trước khi viết code.

Người sở hữu file: TV1. Ngày chốt: 23/11/2026.

## 1. Ngôn ngữ và môi trường

- Toàn bộ chương trình viết bằng **C++17** (giảng viên yêu cầu chỉ dùng C++): 8 thuật toán, sinh dữ liệu, đo đạc, tổng hợp, minh họa.
- Biên dịch: `g++ -O2 -std=c++17`.
- Không gọi thư viện sắp xếp bên trong thuật toán. `std::sort` chỉ dùng để (a) kiểm tra đúng và (b) tạo dữ liệu sorted/reversed trong `gen_data.cpp`.
- Biểu đồ vẽ bằng Google Sheets từ file CSV do chương trình xuất ra (đường lý thuyết bằng công thức).
- Mỗi người ghi cấu hình máy vào `results/machine_TVx.txt`: CPU, RAM, hệ điều hành, `g++ --version`.

## 2. Cấu trúc repo

```
src/      common.h (TV2), qs1.cpp ... qs6.cpp, ms.cpp, hs.cpp
tools/    gen_data.cpp (TV1), aggregate.cpp, run_all.cpp, frame.h, viewer.cpp (TV3)
data/     dữ liệu sinh ra (KHÔNG đẩy lên Git, có thể sinh lại; đã có trong .gitignore)
results/  raw_<ID>.csv, raw_ALL.csv, summary.csv, machine_TVx.txt
figs/     ảnh biểu đồ xuất từ Google Sheets
report/   Phan_TV1.docx, Phan_TV2.docx, Phan_TV3.docx
```

Mỗi người chỉ sửa file của mình. `common.h` do TV2 sở hữu.

## 3. Tám thuật toán

Trục chốt (QS1 đến QS3) cùng dùng phân hoạch Lomuto. Trục phân hoạch (QS4 đến QS6) cùng dùng chốt giữa. Đệ quy thuần cả hai nhánh, **không** chuyển sang Insertion Sort ở đoạn nhỏ.

| ID | Tên | Chốt | Phân hoạch | Phụ trách |
|---|---|---|---|---|
| QS1 | Chốt cố định | `a[hi]` | Lomuto | TV1 |
| QS2 | Chốt ngẫu nhiên | `lo + rng() % (hi-lo+1)` | Lomuto | TV2 |
| QS3 | Median-of-three | trung vị của `a[lo]`, `a[mid]`, `a[hi]` | Lomuto | TV2 |
| QS4 | Phân hoạch Lomuto | `a[(lo+hi)/2]` | Lomuto (hoán chốt về cuối) | TV1 |
| QS5 | Phân hoạch Hoare | `a[(lo+hi)/2]` | Hoare, đệ quy `(lo,j)` và `(j+1,hi)` | TV1 |
| QS6 | Phân hoạch 3 nhánh | `a[(lo+hi)/2]` | Dijkstra: nhỏ hơn, bằng, lớn hơn | TV2 |
| MS | MergeSort top-down | không | mảng phụ cấp phát một lần, trộn ổn định | TV3 |
| HS | HeapSort | không | build-heap + siftDown bằng vòng lặp | TV3 |

Chữ ký hàm của mỗi thuật toán (để `run_all.cpp` gọi chung):

```cpp
void sort_QS1(std::vector<int>& a, unsigned seed);  // tương tự sort_QS2 ... sort_HS
// seed chỉ có tác dụng với QS2; các thuật toán khác bỏ qua.
```

## 4. Quy ước đếm phép toán

Chỉ đếm thao tác trên **phần tử mảng**, không đếm biến chạy vòng lặp.

| Chỉ số | Quy ước |
|---|---|
| `cmp` | Mỗi phép so sánh giữa hai giá trị phần tử mảng = 1. Median-of-three tính 2 đến 3 so sánh khi chọn chốt. |
| `assign` | Mỗi lần ghi vào ô mảng = 1. Một lần `swap` = 3 phép gán. MergeSort tính mọi lần ghi vào mảng phụ và ghi trả về. |
| `calls` | Mỗi lần vào hàm đệ quy = 1, kể cả đoạn rỗng hoặc 1 phần tử. HeapSort lặp nên `calls = 0`. |
| `max_depth` | Độ sâu ngăn xếp đệ quy lớn nhất. |
| `time_ms` | `std::chrono::steady_clock`, không tính thời gian sao chép mảng. |

Mỗi (thuật toán, dữ liệu) chạy **hai lượt riêng**: lượt 1 bật bộ đếm (ghi `cmp/assign/calls/max_depth`), lượt 2 tắt bộ đếm (ghi `time_ms`). Với n ≤ 4000, lặp nội bộ 20 lần rồi chia để giảm nhiễu. Sau mỗi lần chạy so khớp với `std::sort` để gán `ok`.

Đề xuất tên hàm bọc trong `common.h` (TV2 xác nhận hoặc đổi, rồi báo nhóm): `LT(a,b)`, `LE(a,b)`, `GT(a,b)`, `GE(a,b)` (so sánh, tăng `cmp`), `SWAP(a,b)` (tăng `assign` thêm 3), `SET(dst,v)` (ghi 1 phần tử, tăng `assign` thêm 1), và một đối tượng `Scope` tạo ở đầu hàm đệ quy (tăng `calls`, theo dõi `max_depth`).

```cpp
struct Result { double time_ms; long long cmp, assign, calls; int max_depth; bool ok; };
Result run(const std::string& algo_id, std::vector<int> a, unsigned seed);
```

## 5. Bộ dữ liệu (`gen_data.cpp`)

- **8 cỡ:** n = 1000, 2000, 4000, 8000, 16000, 32000, 64000, 128000 (nhân đôi để kiểm tra nhanh bằng tỉ số T(2n)/T(n): khoảng 2 với n log n, khoảng 4 với n²).
- **4 loại:**
  - `random`: số nguyên trong [0, 10^9].
  - `sorted`: sinh như random rồi sắp xếp tăng dần.
  - `reversed`: sinh như random rồi sắp xếp giảm dần.
  - `dup`: chỉ 10 giá trị phân biệt (0 đến 9).
- **5 lần chạy** mỗi cấu hình (rep = 0..4). Seed dữ liệu = `(loại+1)*1000003 + n*31 + rep`. Seed chốt của QS2 = `rep`.
- **Bộ sinh số:** `std::mt19937` và `rng() % M`. **Không** dùng `std::uniform_int_distribution` vì mỗi trình biên dịch cài khác nhau, ba máy sẽ ra dữ liệu khác nhau. Chốt của QS2 cũng dùng `lo + rng() % (hi - lo + 1)`.
- **Tên file:** `data/<loai>_<n>_<rep>.txt`. Dòng 1 là n, dòng 2 là n số cách nhau bởi dấu cách.
- Chạy: `./gen_data data` (đủ bộ, 160 file, khoảng 39 MB) hoặc `./gen_data data_nho 4000` (chỉ cỡ ≤ 4000, để test nhanh).
- Ngăn xếp: QS1 trên sorted/reversed có độ sâu đệ quy bằng n. Chạy với `ulimit -s unlimited` (Linux/WSL) hoặc trong luồng có stack lớn. Nếu vẫn tràn, ghi "N/A (stack overflow)" và đưa vào biện luận.

## 6. Số liệu thô và tổng hợp

Schema CSV (mỗi người xuất `results/raw_<ID>.csv`; chạy chung xuất `results/raw_ALL.csv`):

```
algo,datatype,n,rep,time_ms,cmp,assign,calls,max_depth,ok
```

- `aggregate.cpp` (TV3) đọc mọi CSV, tính trung bình và độ lệch chuẩn theo (algo, datatype, n), ghi `results/summary.csv`.
- Số phép toán không phụ thuộc máy nên gộp trực tiếp từ CSV từng người. **Thời gian** của biểu đồ so sánh chéo 8 thuật toán phải lấy từ một lần chạy chung `run_all` trên **một máy** (máy của TV3, ngày 30/11).

## 7. Quy chuẩn biểu đồ (Google Sheets)

- Trục x là n. Dùng thang log-log khi các đường chênh nhau nhiều bậc. Thanh sai số = độ lệch chuẩn của 5 lần chạy.
- Đường lý thuyết nét đứt c·f(n), f(n) = n·log2(n) hoặc n². Hằng số c = trung bình nhân của T/f(n) ở 4 cỡ lớn nhất (có mẫu ở sheet "Mẫu biểu đồ").
- Màu cố định: QS1 xanh dương `#1F77B4`, QS2 cam `#FF7F0E`, QS3 xanh lá `#2CA02C`, QS4 đỏ `#D62728`, QS5 tím `#9467BD`, QS6 nâu `#8C564B`, MS xám `#7F7F7F`, HS đen `#000000`.
- Tên ảnh: `fig_<metric>_<datatype>_<nhom>.png`, metric ∈ {time, cmp, assign}, nhom ∈ {cum1, cum2, cum3, all}. Mọi hình có số hiệu và chú thích.

## 8. Khung minh họa (viewer)

```cpp
struct Frame { std::vector<int> a; int lo, hi, pivot, i, j; std::string note; };
std::vector<Frame> frames_qs1(std::vector<int> a);  // tương tự frames_qs2 ... frames_hs; chạy trên mảng nhỏ (n ≤ 30)
```

`frame.h` và `viewer.cpp` do TV3 viết (26/11).

## 9. Điểm bàn giao và mốc thời gian

| Ngày | Ai gửi | Gửi gì | Ai nhận |
|---|---|---|---|
| 23/11 | TV1 | STANDARDS.md, gen_data.cpp | TV2, TV3 |
| 23/11 | TV2 | common.h | TV1, TV3 |
| 25/11 | TV1, TV2, TV3 | qs1..qs6, ms, hs .cpp | TV3 (ghép run_all) |
| 28/11 | TV3 | tag **v1.0** (chốt mã nguồn, sau đó chỉ sửa lỗi) | cả nhóm |
| 29/11 | TV1, TV2, TV3 | raw_<ID>.csv, machine_TVx.txt | TV3 |
| 30/11 | TV3 | raw_ALL.csv, summary.csv | TV1, TV2 |

## 10. Liêm chính học thuật

- Số liệu do nhóm tự đo. Nộp kèm `gen_data.cpp` và file CSV thô.
- Mọi nguồn tham khảo và công cụ AI đã dùng ghi vào Tài liệu tham khảo (IEEE).
- Ai cũng phải giải thích được mọi dòng mã và mọi bước chứng minh, kể cả phần của bạn khác.
