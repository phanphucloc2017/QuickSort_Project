#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <filesystem>
// Hỗ trợ xử lý ngoại lệ khi chuyển chuỗi sang số nguyên
#include <stdexcept>

using namespace std;
namespace fs = std::filesystem;

const vector<int> SIZES = {
    1000, 2000, 4000, 8000,
    16000, 32000, 64000, 128000
};

const int REPETITIONS = 5;
const int MIN_RANDOM = 0;
const int MAX_RANDOM = 1000000000;

// Hàm tạo seed dựa trên loại dữ liệu, kích thước và lần lặp
unsigned int make_seed(int type, int n, int rep) {

    // Công thức seed được quy định trong tiêu chuẩn nhóm
    return (type + 1) * 1000003 + n * 31 + rep;
}

// Hàm sinh dữ liệu ngẫu nhiên gồm n phần tử
vector<int> generate_random_data(int n, unsigned int seed) {

    // Khởi tạo bộ sinh số ngẫu nhiên Mersenne Twister
    // Seed cố định giúp có thể tái tạo lại cùng một bộ dữ liệu
    mt19937 rng(seed);

    // Khởi tạo vector chứa n phần tử
    vector<int> data(n);

    // Sinh giá trị ngẫu nhiên cho từng phần tử
    for (int i = 0; i < n; i++) {

        // Sinh số trong khoảng từ 0 đến 1.000.000.000
        data[i] = rng() % (MAX_RANDOM + 1);
    }

    // Trả về vector dữ liệu đã sinh
    return data;
}

// Hàm tạo dữ liệu ngẫu nhiên rồi sắp xếp tăng dần
vector<int> generate_sorted_data(int n, unsigned int seed) {

    // Sinh dữ liệu ngẫu nhiên với seed được cung cấp
    vector<int> data = generate_random_data(n, seed);

    // Sắp xếp các phần tử theo thứ tự tăng dần
    sort(data.begin(), data.end());

    // Trả về vector dữ liệu đã sắp xếp
    return data;
}

// Hàm tạo dữ liệu ngẫu nhiên rồi sắp xếp giảm dần
vector<int> generate_reversed_data(int n, unsigned int seed) {

    // Sinh dữ liệu ngẫu nhiên với seed được cung cấp
    vector<int> data = generate_random_data(n, seed);

    // Sắp xếp dữ liệu theo thứ tự tăng dần trước
    sort(data.begin(), data.end());

    // Đảo ngược vector để có thứ tự giảm dần
    reverse(data.begin(), data.end());

    // Trả về vector dữ liệu đã sắp xếp giảm dần
    return data;
}

// Hàm tạo dữ liệu có nhiều phần tử trùng lặp
vector<int> generate_duplicate_data(int n, unsigned int seed) {

    // Khởi tạo bộ sinh số ngẫu nhiên Mersenne Twister
    mt19937 rng(seed);

    // Khởi tạo vector chứa n phần tử
    vector<int> data(n);

    // Sinh giá trị ngẫu nhiên cho từng phần tử
    for (int i = 0; i < n; i++) {

        // Chỉ sinh các giá trị từ 0 đến 9
        // Vì vậy dữ liệu chỉ có tối đa 10 giá trị khác nhau
        data[i] = rng() % 10;
    }

    // Trả về vector dữ liệu đã sinh
    return data;
}

//////
///////// Hàm ghi một bộ dữ liệu ra file TXT
bool write_data_file(
    const string& output_dir,
    const string& datatype,
    int n,
    int rep,
    const vector<int>& data
) {

    // Tạo tên file theo quy chuẩn: loại_n_rep.txt
    string filename = datatype + "_" 
                    + to_string(n) + "_" 
                    + to_string(rep) + ".txt";

    // Ghép đường dẫn thư mục và tên file
    fs::path filepath = fs::path(output_dir) / filename;

    // Mở file để ghi dữ liệu
    ofstream outfile(filepath);

    // Kiểm tra nếu không thể mở file
    if (!outfile.is_open()) {
        cerr << "Khong the mo file: " << filepath << endl;
        return false;
    }

    // Ghi số lượng phần tử vào dòng đầu tiên
    outfile << n << '\n';

    // Ghi các phần tử trên dòng thứ hai
    for (int i = 0; i < n; i++) {

        // Ghi giá trị của phần tử
        outfile << data[i];

        // Thêm dấu cách giữa các phần tử
        if (i < n - 1) {
            outfile << ' ';
        }
    }

    // Đóng file sau khi ghi xong
    outfile.close();

    // Trả về true nếu ghi dữ liệu thành công
    return true;
}

/////////////
// Hàm điều phối việc sinh toàn bộ bộ dữ liệu
void generate_all_data(const string& output_dir, int max_n) {

    // Tạo thư mục lưu dữ liệu nếu thư mục chưa tồn tại
    fs::create_directories(output_dir);

    // Khai báo tên 4 loại dữ liệu
    const vector<string> TYPES = {
        "random", "sorted", "reversed", "dup"
    };

    // Duyệt qua từng kích thước dữ liệu
    for (int n : SIZES) {

        // Bỏ qua kích thước lớn hơn giới hạn được yêu cầu
        if (n > max_n) {
            continue;
        }

        // Duyệt qua 5 lần lặp
        for (int rep = 0; rep < REPETITIONS; rep++) {

            // Duyệt qua 4 loại dữ liệu
            for (int type = 0; type < 4; type++) {

                // Tạo seed dựa trên loại dữ liệu, kích thước và lần lặp
                unsigned int seed = make_seed(type, n, rep);

                // Khởi tạo vector chứa dữ liệu
                vector<int> data;

                // Sinh dữ liệu theo từng loại
                if (type == 0) {

                    // Sinh dữ liệu ngẫu nhiên
                    data = generate_random_data(n, seed);

                } else if (type == 1) {

                    // Sinh dữ liệu ngẫu nhiên rồi sắp xếp tăng dần
                    data = generate_sorted_data(n, seed);

                } else if (type == 2) {

                    // Sinh dữ liệu ngẫu nhiên rồi sắp xếp giảm dần
                    data = generate_reversed_data(n, seed);

                } else {

                    // Sinh dữ liệu chỉ có tối đa 10 giá trị khác nhau
                    data = generate_duplicate_data(n, seed);
                }

                // Ghi dữ liệu ra file TXT
                if (!write_data_file(
                    output_dir, TYPES[type], n, rep, data
                )) {

                    // Dừng chương trình nếu ghi file thất bại
                    cerr << "Loi khi ghi du lieu!" << endl;
                    return;
                }
            }
        }
    }

    // Thông báo hoàn thành việc sinh dữ liệu
    cout << "Da sinh xong bo du lieu!" << endl;
}

/////////
// Hàm main: điểm bắt đầu thực thi chương trình
int main(int argc, char* argv[]) {

    // Kiểm tra số lượng tham số truyền vào
    if (argc < 2 || argc > 3) {

        // Hiển thị hướng dẫn sử dụng chương trình
        cerr << "Cach su dung: gen_data <thu_muc> [max_n]" << endl;

        // Kết thúc chương trình với mã lỗi
        return 1;
    }

    // Lấy tên thư mục lưu dữ liệu từ tham số đầu tiên
    string output_dir = argv[1];

    // Mặc định sử dụng kích thước lớn nhất trong danh sách
    int max_n = SIZES.back();

    // Kiểm tra nếu người dùng có nhập giới hạn kích thước
    if (argc == 3) {

        try {

            // Chuyển tham số thứ hai thành số nguyên
            max_n = stoi(argv[2]);

        } catch (const exception& e) {

            // Thông báo nếu tham số không hợp lệ
            cerr << "Gia tri max_n khong hop le!" << endl;

            // Kết thúc chương trình với mã lỗi
            return 1;
        }
    }

    // Kiểm tra giới hạn kích thước phải lớn hơn 0
    if (max_n <= 0) {
        cerr << "max_n phai lon hon 0!" << endl;
        return 1;
    }

    // Thông báo bắt đầu sinh dữ liệu
    cout << "Bat dau sinh du lieu..." << endl;

    // Gọi hàm điều phối để sinh toàn bộ dữ liệu
    generate_all_data(output_dir, max_n);

    // Thông báo chương trình kết thúc
    return 0;
}