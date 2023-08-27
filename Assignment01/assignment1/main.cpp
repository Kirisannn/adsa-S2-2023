#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

string makeString(vector<int> v) {  // Takes a int vector and returns a string
    // Get the index of the first non-zero element
    size_t firstNonZero = 0;
    for (size_t i = 0; i < v.size(); i++) {
        if (v[i] != 0) {
            firstNonZero = i;
            break;
        }
    }

    // From the first non-zero element, make a string
    string s = "";
    for (size_t i = firstNonZero; i < v.size(); i++) {
        s += to_string(v[i]);
    }

    return s;
}

vector<int> schoolAddition(vector<int> x1, vector<int> x2, int base, size_t n1, size_t n2, size_t n) {
    vector<int> carry, sum;
    for (size_t i = 0; i < n + 1; i++) {  // Zero carry & sum to the size of n + 1 (for the last carry)
        carry.push_back(0);
        sum.push_back(0);
    }

    for (size_t i = 0; i < n; i++) {  // Add x1 and x2
        sum[i] = x1[i] + x2[i];
        if (sum[i] >= base) {
            carry[i + 1] = sum[i] / base;
            sum[i] = sum[i] % base;
        }
        sum[i] += carry[i];
        if (sum[i] >= base) {
            carry[i + 1] = sum[i] / base;
            sum[i] = sum[i] % base;
        }

        if (i == n - 1) {
            sum[i + 1] += carry[i + 1];
        }
    }

    // Reverse the sum
    reverse(sum.begin(), sum.end());

    return sum;
}

vector<int> schoolMultiplication(vector<int> x1, vector<int> x2, int base, size_t n1, size_t n2, size_t n) {
    vector<int> temp, product, carry;
    for (size_t i = 0; i < 2 * n; i++) {  // Zero temp & carry to the size of 2*n
        temp.push_back(0);
        product.push_back(0);
        carry.push_back(0);
    }

    // Calculate the product
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {  // Multiply x1[i] and x2[j]
            temp[i + j] = x1[j] * x2[i];
            if (temp[i + j] >= base) {
                carry[i + j + 1] = temp[i + j] / base;
                temp[i + j] = temp[i + j] % base;
            }
        }

        // Sum temp and carry, and add it to product
        temp = schoolAddition(temp, carry, base, temp.size(), carry.size(), temp.size());
        reverse(temp.begin(), temp.end());
        temp.pop_back();
        product = schoolAddition(product, temp, base, product.size(), temp.size(), product.size());
        reverse(product.begin(), product.end());
        product.pop_back();

        // Zero temp and carry
        for (size_t k = 0; k < 2 * n; k++) {
            temp[k] = 0;
            carry[k] = 0;
        }
    }

    reverse(product.begin(), product.end());

    return product;
};

vector<int> karatsuba(vector<int> x1, vector<int> x2, int base, size_t n1, size_t n2, size_t n) {
    vector<int> karatsubaProduct;
    return karatsubaProduct;
};

int main() {
    string I1, I2, B;
    cin >> I1 >> I2 >> B;
    int base = stoi(B);

    // Get lengths of I1, I2 and the max length as n
    size_t n1 = I1.length(), n2 = I2.length(), n = max(n1, n2);

    // Convert I1, I2 into integer vectors
    vector<int> x1, x2;
    for (size_t i = 0; i < n; i++) {  // Zero the vectors
        x1.push_back(0);
        x2.push_back(0);
    }
    // Get the smaller integer. It will be padded with zeros
    if (n1 > n2) {                         // If I1 is larger, pad I2 with zeros
        for (size_t i = 0; i < n1; i++) {  // Add I1 to x1 normally
            x1[i] = stoi(I1.substr(i, 1));
        }
        for (size_t i = 0; i < n2; i++) {  // Add I2 to x2 from the last - i position
            x2[n - n2 + i] = stoi(I2.substr(i, 1));
        }
    } else if (n1 < n2) {                  // If I2 is larger, pad I1 with zeros
        for (size_t i = 0; i < n1; i++) {  // Add I1 to x1 from the last - i position
            x1[n - n1 + i] = stoi(I1.substr(i, 1));
        }
        for (size_t i = 0; i < n2; i++) {  // Add I2 to x2 normally
            x2[i] = stoi(I2.substr(i, 1));
        }
    } else {                               // If both are equal, add normally
        for (size_t i = 0; i < n1; i++) {  // Add I1 to x1 normally
            x1[i] = stoi(I1.substr(i, 1));
        }
        for (size_t i = 0; i < n2; i++) {  // Add I2 to x2 normally
            x2[i] = stoi(I2.substr(i, 1));
        }
    }

    // Reverse the vectors
    reverse(x1.begin(), x1.end());
    reverse(x2.begin(), x2.end());

    // School Addition
    vector<int> sum = schoolAddition(x1, x2, base, n1, n2, n);
    string sumString = makeString(sum);
    cout << sumString << " ";

    // vector<int> product = schoolMultiplication(x1, x2, base, n1, n2, n);  // School Multiplication
    vector<int> product = karatsuba(x1, x2, base, n1, n2, n);  // Karatsuba Multiplication
    string prodString = makeString(product);
    cout << prodString << " ";

    cout << "0" << endl;

    return 0;
}