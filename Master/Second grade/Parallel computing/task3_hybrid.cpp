#include <cmath>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <cstdlib>
#include <mpi.h>
#include <cmath>
#include <omp.h>

double u(double x, double y) {
    return 2.0 / (1.0 + pow(x, 2) + pow(y, 2));
}

double F(double x, double y) {
    return 32.0 * x * y / pow(1.0 + pow(x, 2) + pow(y, 2), 3)
           + 8.0 / pow(1.0 + pow(x, 2) + pow(y, 2), 2)
           + u(x, y);
}

double psi(double x, double y) {
    return -4.0 * (1.0 + pow(x + y, 2)) / pow(1.0 + pow(x, 2) + pow(y, 2), 2);
}

double k(double x, double y) {
    return 1 + pow(x + y, 2);
}

const double X_LOWER = -2.0;
const double X_UPPER = 3.0;
const double Y_LOWER = -1.0;
const double Y_UPPER = 4.0;

double **init_matrix(int w, int h) {
    double **result = new double *[w + 1];
    for (int i = 0; i <= w; i++) {
        result[i] = new double[h + 1];
        memset(result[i], 0, h * sizeof(double));
    }
    return result;
}

void fill_b_matrix(double **arr, int M, int N, double h_1, double h_2) {
    // B MATRIX (right side of equation)

    // CENTER
    #pragma omp parallel for num_threads(4)
    for (int m = 1; m < M; ++m) {
        for (int n = 1; n < N; ++n) {
            arr[m][n] = F(X_LOWER + m * h_1, Y_LOWER + n * h_2);
        }
    }
    // TOP BORDER
    #pragma omp parallel for num_threads(4)
    for (int m = 1; m < M; ++m) {
        arr[m][N] = F(X_LOWER + m * h_1, Y_UPPER) + Y_UPPER * psi(X_LOWER + m * h_1, Y_UPPER) * 2.0 / h_2;
    }
    // BOTTOM BORDER
    #pragma omp parallel for num_threads(4)
    for (int m = 1; m < M; ++m) {
        arr[m][0] = F(X_LOWER + m * h_1, Y_LOWER) - Y_LOWER * psi(X_LOWER + m * h_1, Y_LOWER) * 2.0 / h_2;
    }
    // RIGHT BORDER
    #pragma omp parallel for num_threads(4)
    for (int n = 1; n < N; ++n) {
        arr[M][n] = F(X_UPPER, Y_LOWER + n * h_2) + X_UPPER * psi(X_UPPER, Y_LOWER + n * h_2) * 2.0 / h_1;
    }
    // LEFT BORDER
    #pragma omp parallel for num_threads(4)
    for (int n = 1; n < N; ++n) {
        arr[0][n] = F(X_LOWER, Y_LOWER + n * h_2) - X_LOWER * psi(X_LOWER, Y_LOWER + n * h_2) * 2.0 / h_1;
    }
    // CORNERS
    arr[0][0] = F(X_LOWER, Y_LOWER) + 2.0 * psi(X_LOWER, Y_LOWER) * (-X_LOWER / h_1 - Y_LOWER / h_2);
    arr[M][0] = F(X_UPPER, Y_LOWER) + 2.0 * psi(X_UPPER, Y_LOWER) * (X_UPPER / h_1 - Y_LOWER / h_2);
    arr[0][N] = F(X_LOWER, Y_UPPER) + 2.0 * psi(X_LOWER, Y_UPPER) * (-X_LOWER / h_1 + Y_UPPER / h_2);
    arr[M][N] = F(X_UPPER, Y_UPPER) + 2.0 * psi(X_UPPER, Y_UPPER) * (X_UPPER / h_1 + Y_UPPER / h_2);
}

void print_matrix(double **arr, int M, int N) {
    std::cout << "Test" << std::endl;
    for (int n = N; n >= 0; --n) {
        for (int m = 0; m <= M; ++m) {
            std::cout << std::setw(10) << arr[m][n] << "\t";
        }
        std::cout << std::endl;
    }
}

void mul_by_A(double **result, double **w, int M, int N, double h_1, double h_2,
              int m_min, int m_max, int n_min, int n_max) {
    // Left side of equation

    // CORNERS
    if (m_min == 0 && n_min == 0) {
        result[0][0] = -(2.0 * k(X_LOWER + 0.5 * h_1, Y_LOWER) * (w[1][0] - w[0][0]) / h_1 / h_1)
                       - (2.0 * k(X_LOWER, Y_LOWER + 0.5 * h_2) * (w[0][1] - w[0][0]) / h_2 / h_2)
                       + w[0][0];
    }
    if (m_max == M && n_min == 0) {
        result[M][0] = (2.0 * k(X_UPPER - 0.5 * h_1, Y_LOWER) * (w[M][0] - w[M - 1][0]) / h_1 / h_1)
                       - (2.0 * k(X_UPPER, Y_LOWER + 0.5 * h_2) * (w[M][1] - w[M][0]) / h_2 / h_2)
                       + w[M][0];
    }
    if (m_min == 0 && n_max == N) {
        result[0][N] = -(2.0 * k(X_LOWER + 0.5 * h_1, Y_UPPER) * (w[1][N] - w[0][N]) / h_1 / h_1)
                       + (2.0 * k(X_LOWER, Y_UPPER - 0.5 * h_2) * (w[0][N] - w[0][N - 1]) / h_2 / h_2)
                       + w[0][N];
    }
    if (m_max == M && n_max == N) {
        result[M][N] = (2.0 * k(X_UPPER - 0.5 * h_1, Y_UPPER) * (w[M][N] - w[M - 1][N]) / h_1 / h_1)
                       + (2.0 * k(X_UPPER, Y_UPPER - 0.5 * h_2) * (w[M][N] - w[M][N - 1]) / h_2 / h_2)
                       + w[M][N];
    }
    // TOP BORDER
    if (n_max == N) {
        #pragma omp parallel for num_threads(4)
        for (int m = std::max(m_min, 1); m < m_max; ++m) {
            result[m][N] = (2 * k(X_LOWER + m * h_1, Y_UPPER - 0.5 * h_2) * (w[m][N] - w[m][N - 1]) / h_2 / h_2)
                           + w[m][N]
                           - (
                                     k(X_LOWER + m * h_1 + 0.5 * h_1, Y_UPPER)
                                     * (w[m + 1][N] - w[m][N]) / h_1
                                     - k(X_LOWER + m * h_1 - 0.5 * h_1, Y_UPPER) *
                                       (w[m][N] - w[m - 1][N]) / h_1
                             ) / h_1;
        }
    }
    // BOTTOM BORDER
    if (n_min == 0) {
        #pragma omp parallel for num_threads(4)
        for (int m = std::max(m_min, 1); m < m_max; ++m) {
            result[m][0] = (2 * k(X_LOWER + m * h_1, Y_LOWER + 0.5 * h_2) * (w[m][0] - w[m][1]) / h_2 / h_2)
                           + w[m][0]
                           - (
                                     k(X_LOWER + m * h_1 + 0.5 * h_1, Y_LOWER)
                                     * (w[m + 1][0] - w[m][0]) / h_1
                                     - k(X_LOWER + m * h_1 - 0.5 * h_1, Y_LOWER) *
                                       (w[m][0] - w[m - 1][0]) / h_1
                             ) / h_1;
        }
    }
    // RIGHT BORDER
    if (m_max == M) {
        #pragma omp parallel for num_threads(4)
        for (int n = std::max(n_min, 1); n < n_max; ++n) {
            result[M][n] = (2 * k(X_UPPER - 0.5 * h_1, Y_LOWER + n * h_2) * (w[M][n] - w[M - 1][n]) / h_1 / h_1)
                           + w[M][n]
                           - (
                                     k(X_UPPER, Y_LOWER + n * h_2 + 0.5 * h_2) *
                                     (w[M][n + 1] - w[M][n]) / h_2
                                     - k(X_UPPER, Y_LOWER + n * h_2 - 0.5 * h_2) *
                                       (w[M][n] - w[M][n - 1]) / h_2
                             ) / h_2;
        }
    }
    // LEFT BORDER
    if (m_min == 0) {
        #pragma omp parallel for num_threads(4)
        for (int n = std::max(n_min, 1); n < n_max; ++n) {
            result[0][n] = (2 * k(X_LOWER + 0.5 * h_1, Y_LOWER + n * h_2) * (w[0][n] - w[1][n]) / h_1 / h_1)
                           + w[0][n]
                           - (
                                     k(X_LOWER, Y_LOWER + n * h_2 + 0.5 * h_2) *
                                     (w[0][n + 1] - w[0][n]) / h_2
                                     - k(X_LOWER, Y_LOWER + n * h_2 - 0.5 * h_2) *
                                       (w[0][n] - w[0][n - 1]) / h_2
                             ) / h_2;
        }
    }
    // CENTER
    #pragma omp parallel for num_threads(4)
    for (int m = std::max(m_min, 1); m < m_max; ++m) {
        for (int n = std::max(n_min, 1); n < n_max; ++n) {
            double delta_h_w_mn = (
                                          k(X_LOWER + m * h_1 + 0.5 * h_1, Y_LOWER + n * h_2)
                                          * (w[m + 1][n] - w[m][n]) / h_1
                                          - k(X_LOWER + m * h_1 - 0.5 * h_1, Y_LOWER + n * h_2) *
                                            (w[m][n] - w[m - 1][n]) / h_1
                                  ) / h_1
                                  + (
                                            // bw_y
                                            k(X_LOWER + m * h_1, Y_LOWER + n * h_2 + 0.5 * h_2) *
                                            (w[m][n + 1] - w[m][n]) / h_2
                                            - k(X_LOWER + m * h_1, Y_LOWER + n * h_2 - 0.5 * h_2) *
                                              (w[m][n] - w[m][n - 1]) / h_2
                                    ) / h_2;
            result[m][n] = w[m][n] - delta_h_w_mn;
        }
    }
}

void matrix_minus_matrix(double **result, double **left, double **right, int M, int N,
                         int m_min, int m_max, int n_min, int n_max) {
    if (m_max != M)
        --m_max;
    if (n_max != N)
        --n_max;
    #pragma omp parallel for num_threads(4)
    for (int m = m_min; m <= m_max; ++m) {
        for (int n = n_min; n <= n_max; ++n) {
            result[m][n] = left[m][n] - right[m][n];
        }
    }
}

double scalar_product(double **left, double **right, int M, int N, double h_1, double h_2,
                      int m_min, int m_max, int n_min, int n_max) {
    double result = 0;
    for (int m = std::max(m_min, 1); m < m_max; ++m) {
        result += left[m][0] * right[m][0] * 0.5;
        for (int n = std::max(n_min, 1); n < n_max; ++n) {
            result += left[m][n] * right[m][n];
        }
        result += left[m][N] * right[m][N] * 0.5;
    }
    if (m_min == 0) {
        for (int n = std::max(n_min, 1); n < n_max; ++n) {
            result += left[0][n] * right[0][n] * 0.5;
        }
    }
    if (m_max == M) {
        for (int n = std::max(n_min, 1); n < n_max; ++n) {
            result += left[M][n] * right[M][n] * 0.5;
        }
    }
    if (n_min == 0) {
        for (int m = std::max(m_min, 1); m < m_max; ++m) {
            result += left[m][0] * right[m][0] * 0.5;
        }
    }
    if (n_max == N) {
        for (int m = std::max(m_min, 1); m < m_max; ++m) {
            result += left[m][N] * right[m][N] * 0.5;
        }
    }
    if (m_min == 0 && n_min == 0)
        result += left[0][0] * right[0][0] * 0.25;
    if (m_min == 0 && n_max == N)
        result += left[0][N] * right[0][N] * 0.25;
    if (m_max == M && n_min == 0)
        result += left[M][0] * right[M][0] * 0.25;
    if (m_max == M && n_max == N)
        result += left[M][N] * right[M][N] * 0.25;
    return result * h_1 * h_2;
}

void matrix_mul(double **result, double **arr, double num, int M, int N,
                int m_min, int m_max, int n_min, int n_max) {
    if (m_max != M)
        --m_max;
    if (n_max != N)
        --n_max;
    #pragma omp parallel for num_threads(4)
    for (int m = m_min; m <= m_max; ++m) {
        for (int n = n_min; n <= n_max; ++n) {
            result[m][n] = arr[m][n] * num;
        }
    }
}

double matrix_max_norm(double **arr, int M, int N,
                       int m_min, int m_max, int n_min, int n_max) {
    if (m_max != M)
        --m_max;
    if (n_max != N)
        --n_max;
    double result = 0.0;
    for (int m = m_min; m <= m_max; ++m) {
        for (int n = n_min; n <= n_max; ++n) {
            result = fmax(result, fabs(arr[m][n]));
        }
    }
    return result;
}
void get_bounds(int &m_min, int &m_max, int &n_min, int &n_max,
                int &bottom_domain, int &top_domain, int &left_domain, int &right_domain,
                int &x_width, int &y_height,
                int M, int N, int proc_num, int proc_id) {
    int x_count;
    for (x_count = sqrt(proc_num); proc_num % x_count != 0; --x_count) {}
    int y_count = proc_num / x_count;
    int x_num = proc_id % x_count;
    int y_num = proc_id / x_count;
    if (!x_num)
        m_min = 0;
    else if (x_num <= (M + 1) % x_count)
        m_min = ((M + 1) / x_count) * x_num + x_num;
    else
        m_min = ((M + 1) / x_count) * x_num + (M + 1) % x_count;

    if (x_num == x_count - 1)
        m_max = M;
    else if (x_num < (M + 1) % x_count)
        m_max = ((M + 1) / x_count) * (x_num + 1) + (x_num + 1);
    else
        m_max = ((M + 1) / x_count) * (x_num + 1) + (M + 1) % x_count;

    if (!y_num)
        n_min = 0;
    else if (y_num <= (N + 1) % y_count)
        n_min = ((N + 1) / y_count) * y_num + y_num;
    else
        n_min = ((N + 1) / y_count) * y_num + (N + 1) % y_count;

    if (y_num == y_count - 1)
        n_max = N;
    else if (y_num < (M + 1) % y_count)
        n_max = ((N + 1) / y_count) * (y_num + 1) + (y_num + 1);
    else
        n_max = ((N + 1) / y_count) * (y_num + 1) + (N + 1) % y_count;

    bottom_domain = proc_id - x_count;
    top_domain = proc_id + x_count;
    left_domain = proc_id - 1;
    right_domain = proc_id + 1;
    x_width = m_max - m_min;
    y_height = n_max - n_min;
}

int main(int argc, char *argv[]) {
    char *p;
    if (argc != 4) {
        std::cout << "Required params: precision net_width net_height";
        return 0;
    }
    int total_iters = 1;
    double precision_desired = std::strtod(argv[1], &p);
    int M = std::strtol(argv[2], &p, 10);
    int N = std::strtol(argv[3], &p, 10);

    double h_1 = (X_UPPER - X_LOWER) / M;
    double h_2 = (Y_UPPER - Y_LOWER) / N;
    double **B = init_matrix(M, N);
    double **w = init_matrix(M, N);
    double **w_next = init_matrix(M, N);
    double **Aw = init_matrix(M, N);
    double **r = init_matrix(M, N);
    double **Ar = init_matrix(M, N);
    double **solution = init_matrix(M, N);
    double **step = init_matrix(M, N);
//    double **diff = init_matrix(M, N);
    double iter_param;
    #pragma omp parallel for num_threads(4)
    for (int m = 0; m <= M; ++m) {
        for (int n = 0; n <= N; ++n) {
            solution[m][n] = u(X_LOWER + m * h_1, Y_LOWER + n * h_2);
        }
    }
    double **discrepancy = init_matrix(M, N);
    double precision = 1.0;
    double precision_square;
    fill_b_matrix(B, M, N, h_1, h_2);

    // Data init finished, starting MPI init
    int num_procs, my_rank, provided;
//    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    double start = MPI_Wtime();

    int m_min, m_max, n_min, n_max, bottom_domain, top_domain, left_domain, right_domain, x_width, y_height;
    get_bounds(m_min, m_max, n_min, n_max,
               bottom_domain, top_domain, left_domain, right_domain,
               x_width, y_height,
               M, N, num_procs, my_rank);
    double *buffer_top = new double[x_width + 16];
    double *buffer_bottom = new double[x_width + 16];
    double *buffer_left = new double[y_height + 16];
    double *buffer_right = new double[y_height + 16];
    double *buffer_top_r = new double[x_width + 16];
    double *buffer_bottom_r = new double[x_width + 16];
    double *buffer_left_r = new double[y_height + 16];
    double *buffer_right_r = new double[y_height + 16];
    MPI_Request send_status[4];
    MPI_Status recv_status[4];

    for (int i = 0; precision > precision_desired; ++i) {
        // sending bottom border
        if (n_min != 0) {
            #pragma omp parallel for num_threads(4)
            for (int j = 0; j <= x_width; ++j)
                buffer_bottom[j] = w[j + m_min][n_min];
            MPI_Isend(buffer_bottom, x_width + 1, MPI_DOUBLE, bottom_domain, 123, MPI_COMM_WORLD, &send_status[0]);
        }
        // sending top border
        if (n_max != N) {
            #pragma omp parallel for num_threads(4)
            for (int j = 0; j <= x_width; ++j)
                buffer_top[j] = w[j + m_min][n_max - 1];
            MPI_Isend(buffer_top, x_width + 1, MPI_DOUBLE, top_domain, 123, MPI_COMM_WORLD, &send_status[1]);
        }
        // sending left border
        if (m_min != 0) {
            #pragma omp parallel for num_threads(4)
            for (int j = 0; j <= y_height; ++j)
                buffer_left[j] = w[m_min][n_min + j];
            MPI_Isend(buffer_left, y_height + 1, MPI_DOUBLE, left_domain, 123, MPI_COMM_WORLD, &send_status[2]);
        }
        // sending right border
        if (m_max != M) {
            #pragma omp parallel for num_threads(4)
            for (int j = 0; j <= y_height; ++j)
                buffer_right[j] = w[m_max - 1][n_min + j];
            MPI_Isend(buffer_right, y_height + 1, MPI_DOUBLE, right_domain, 123, MPI_COMM_WORLD, &send_status[3]);
        }
        //receiving bottom border
        if (n_min != 0) {
            MPI_Recv(buffer_bottom_r, x_width + 1, MPI_DOUBLE, bottom_domain, 123, MPI_COMM_WORLD, &recv_status[0]);
            #pragma omp parallel for num_threads(4)
            for (int j = 0; j <= x_width; ++j)
                w[m_min + j][n_min - 1] = buffer_bottom_r[j];
        }
        //receiving top border
        if (n_max != N) {
            MPI_Recv(buffer_top_r, x_width + 1, MPI_DOUBLE, top_domain, 123, MPI_COMM_WORLD, &recv_status[1]);
            #pragma omp parallel for num_threads(4)
            for (int j = 0; j <= x_width; ++j)
                w[m_min + j][n_max] = buffer_top_r[j];
        }
        //receiving left border
        if (m_min != 0) {
            MPI_Recv(buffer_left_r, y_height + 1, MPI_DOUBLE, left_domain, 123, MPI_COMM_WORLD, &recv_status[2]);
            #pragma omp parallel for num_threads(4)
            for (int j = 0; j <= y_height; ++j)
                w[m_min - 1][n_min + j] = buffer_left_r[j];
        }
        //receiving right border
        if (m_max != M) {
            MPI_Recv(buffer_right_r, y_height + 1, MPI_DOUBLE, right_domain, 123, MPI_COMM_WORLD, &recv_status[3]);
            #pragma omp parallel for num_threads(4)
            for (int j = 0; j < y_height; ++j)
                w[m_max][n_min + j] = buffer_right_r[j];
        }
        matrix_minus_matrix(discrepancy, w, solution, M, N, m_min, m_max, n_min, n_max);
        mul_by_A(Aw, w, M, N, h_1, h_2, m_min, m_max, n_min, n_max);
        matrix_minus_matrix(r, Aw, B, M, N, m_min, m_max, n_min, n_max);
        mul_by_A(Ar, r, M, N, h_1, h_2, m_min, m_max, n_min, n_max);
        iter_param = scalar_product(Ar, r, M, N, h_1, h_2, m_min, m_max, n_min, n_max)
                     / scalar_product(Ar, Ar, M, N, h_1, h_2, m_min, m_max, n_min, n_max);
        matrix_mul(step, r, iter_param, M, N, m_min, m_max, n_min, n_max);
        matrix_minus_matrix(w_next, w, step, M, N, m_min, m_max, n_min, n_max);
        //precision = scalar_product(step, step, M, N, h_1, h_2, m_min, m_max, n_min, n_max);
        precision = matrix_max_norm(step, M, N, m_min, m_max, n_min, n_max);
        MPI_Allreduce(&precision, &precision_square, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        //precision = sqrt(precision_square);
        precision = precision_square / num_procs;
        std::cout << "Precision: " << std::setprecision(10) << precision << std::endl;
        std::swap(w, w_next);
        total_iters += 1;
    }
    MPI_Barrier(MPI_COMM_WORLD);
    double time_spent = MPI_Wtime() - start;
    double max_time_spent;
    MPI_Reduce(&time_spent, &max_time_spent, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Finalize();
    if (my_rank == 0) {
//        print_matrix(discrepancy, M, N);
        std::cout << "Time spent: " << std::setprecision(10) << max_time_spent << std::endl;
        std::cout << "Iterations: " << std::setprecision(10) << total_iters << std::endl;
        std::cout << "Precision: " << std::setprecision(10) << precision << std::endl;
        std::cout << "Run params:\n" << std::setprecision(10) <<
        "Num processes: " << num_procs <<
        "\nM=" << M <<
        "\nN=" << N << std::endl;
    }
}