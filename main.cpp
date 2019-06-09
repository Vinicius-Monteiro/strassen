#include <iostream>

using namespace std;

void print(const int &n, const int &m, int **matrix);

void fill(const int &n, const int &m, const int &starting_n, const int &starting_m, int **matrix);

int** unfill(int n_a, int m_b, int n, int **c);

int** matrix_sum(int n, int **a, int **b);

int** matrix_subtraction(int n, int **a, int **b);

unsigned int next_power_of_two(unsigned int v);

int **create_matrix(int n);

void put_submatrix(int n_start, int m_start, int n, int **m, int **c);

void get_submatrix(int n_start, int m_start, int n, int **m, int **c);

int** strassen(int n, int n_start, int m_start, int **a, int **b);

int main(){
	unsigned int n_a, m_a, n_b, m_b;
	cin >> n_a >> m_a >> n_b >> m_b;

	unsigned int a_size, b_size, size;
	if(n_a != m_a)
		a_size = (n_a > m_a) ? next_power_of_two(n_a) : next_power_of_two(m_a);
	else
		a_size = next_power_of_two(n_a);

	if(n_b != m_b)
		b_size = (n_b > m_b) ? next_power_of_two(n_b) : next_power_of_two(m_b);
	else
		b_size = next_power_of_two(n_b);
	
	size = (a_size >= b_size) ? a_size : b_size;

	int **a = create_matrix(size);
	int **b = create_matrix(size);

	for(int i = 0; i < n_a; i++)
		for(int j = 0; j < m_a; j++)
			cin >> a[i][j];
	for(int i = 0; i < n_b; i++)
		for(int j = 0; j < m_b; j++)
			cin >> b[i][j];
			
	fill(size, size, n_a, m_a, a);
	fill(size, size, n_b, m_b, b);

	int **c = strassen(size, 0, 0, a, b);
	c = unfill(n_a, m_b, size, c);

	cout << "18110528" << endl << n_a << " " << m_b << endl;
	print(n_a, m_b, c);

	return 0;
}

void print(const int &n, const int &m, int **matrix){
	for(int i = 0; i < n; i++){
		for(int j = 0; j < m; j++)
			cout << matrix[i][j] << " ";
		cout << endl;
	}
}

void fill(const int &n, const int &m, const int &starting_n, const int &starting_m, int **matrix){
	for(int i = starting_n; i < n; i++)
		for(int j = starting_m; j < m; j++)
			matrix[i][j] = 0;
}

int** unfill(int n_a, int m_b, int n, int **c){
	int **aux = new int*[n_a];
	for(int i = 0; i < n_a; i++)
		aux[i] = new int[m_b];

	for(int i = 0; i < n_a; i++)
		for(int j = 0; j < m_b; j++)
			aux[i][j] = c[i][j];

	for(int i = 0; i < n; i++)
		delete c[i];
	delete[] c;

	return aux;
}

int** matrix_sum(int n, int **a, int **b){
	int **c = create_matrix(n);
	for(int i = 0; i < n; i++)
		for(int j = 0; j < n; j++)
			c[i][j] = a[i][j] + b[i][j];
	return c;
}

int** matrix_subtraction(int n, int **a, int **b){
	int **c = create_matrix(n);
	for(int i = 0; i < n; i++)
		for(int j = 0; j < n; j++)
			c[i][j] = a[i][j] - b[i][j];
	return c;
}

unsigned int next_power_of_two(unsigned int v){
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	return ++v;
}

int** create_matrix(int n){
	int **a = new int*[n];
	for(int i = 0; i < n; i++)
		a[i] = new int[n];
	return a;
}

void put_submatrix(int n_start, int m_start, int n, int **m, int **c){
	for(int i = 0; i < n; i++)
		for(int j = 0; j < n; j++)
			c[i + n_start][j + m_start] = m[i][j];
}

void get_submatrix(int n_start, int m_start, int n, int **m, int **c){
	for(int i = 0; i < n; i++)
		for(int j = 0; j < n; j++)
			c[i][j] = m[i + n_start][j + m_start];
}

int** strassen(int n, int n_start, int m_start, int **a, int **b){
	int **c = create_matrix(n);
	if(n == 1){ 
		c[0][0] = a[0][0] * b[0][0];
		return c;
	}

	int **a11 = create_matrix(n/2);
	get_submatrix(n_start, m_start, n/2, a, a11);

	int **a12 = create_matrix(n/2);
	get_submatrix(n_start, m_start + n/2, n/2, a, a12);

	int **a21 = create_matrix(n/2);
	get_submatrix(n_start + n/2, m_start, n/2, a, a21);

	int **a22 = create_matrix(n/2);
	get_submatrix(n_start + n/2, m_start + n/2, n/2, a, a22);

	int **b11 = create_matrix(n/2);
	get_submatrix(n_start, m_start, n/2, b, b11);

	int **b12 = create_matrix(n/2);
	get_submatrix(n_start, m_start + n/2, n/2, b, b12);

	int **b21 = create_matrix(n/2);
	get_submatrix(n_start + n/2, m_start, n/2, b, b21);

	int **b22 = create_matrix(n/2);
	get_submatrix(n_start + n/2, m_start + n/2, n/2, b, b22);
	
	int **m1 = create_matrix(n/2);
	m1 = strassen(n/2, 0, 0, matrix_sum(n/2, a11, a22), matrix_sum(n/2, b11, b22));
	int **m2 = create_matrix(n/2);
	m2 = strassen(n/2, 0, 0, matrix_sum(n/2, a21, a22), b11);
	int **m3 = create_matrix(n/2);
	m3 = strassen(n/2, 0, 0, a11, matrix_subtraction(n/2, b12, b22));
	int **m4 = create_matrix(n/2);
	m4 = strassen(n/2, 0, 0, a22, matrix_subtraction(n/2, b21, b11));
	int **m5 = create_matrix(n/2);
	m5 = strassen(n/2, 0, 0, matrix_sum(n/2, a11, a12), b22);
	int **m6 = create_matrix(n/2);
	m6 = strassen(n/2, 0, 0, matrix_subtraction(n/2, a21, a11), matrix_sum(n/2, b11, b12));
	int **m7 = create_matrix(n/2);
	m7 = strassen(n/2, 0, 0, matrix_subtraction(n/2, a12, a22), matrix_sum(n/2, b21, b22));

	int **c11 = create_matrix(n/2);
	c11 = matrix_sum(n/2, matrix_subtraction(n/2, matrix_sum(n/2, m1, m4), m5), m7);
	int **c12 = create_matrix(n/2);
	c12 = matrix_sum(n/2, m3, m5);
	int **c21 = create_matrix(n/2);
	c21 = matrix_sum(n/2, m2, m4);
	int **c22 = create_matrix(n/2);
	c22 = matrix_sum(n/2, matrix_sum(n/2, matrix_subtraction(n/2, m1, m2), m3), m6);

	put_submatrix(n_start, m_start, n/2, c11, c);
	put_submatrix(n_start, m_start + n/2, n/2, c12, c);
	put_submatrix(n_start + n/2, m_start, n/2, c21, c);
	put_submatrix(n_start + n/2, m_start + n/2, n/2, c22, c);

	return c;
}