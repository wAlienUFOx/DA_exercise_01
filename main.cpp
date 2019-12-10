#include <iostream>
#include <algorithm>
#include <cassert>
#include <iostream>


namespace NVec {

template <typename T>
class TVector {
public:
    using value_type = T;
    using iterator = value_type*;
    using const_iterator = const value_type*;
  
    TVector():
        Already_used_(0), Storage_size_(0), Storage_(nullptr){
    }

    TVector(int size, const value_type& default_value = value_type()): TVector(){
        assert(size >= 0);

        if (size == 0)
	    return;

        Already_used_ = size;
        Storage_size_ = size;
        Storage_ = new value_type[size];

        std::fill(Storage_, Storage_ + Already_used_, default_value);
    }

    int Size() const{
        return Already_used_;
    }

    bool Empty() const{
        return Size() == 0;
    }

    iterator begin() const{
        return Storage_;
    }

    iterator end() const{
        if (Storage_)
	    return Storage_ + Already_used_;

        return nullptr;
    }

    friend void swap(TVector& lhs, TVector& rhs){
        using std::swap;

        swap(lhs.Already_used_, rhs.Already_used_);
        swap(lhs.Storage_size_, rhs.Storage_size_);
        swap(lhs.Storage_, rhs.Storage_);
    }

    TVector& operator=(TVector other){
        swap(*this, other);
        return *this;
    }

    TVector(const TVector& other): TVector() {
        TVector next(other.Storage_size_);
        next.Already_used_ = other.Already_used_;

        if (other.Storage_ )
            std::copy(other.Storage_, other.Storage_ + other.Storage_size_,
                      next.Storage_);

        swap(*this, next);
    }

    ~TVector() {
        delete[] Storage_;

        Storage_size_ = 0;
        Already_used_ = 0;
        Storage_ = nullptr;
    }

    void Clear() {
        delete [] this -> Storage_;
	this->Storage_size_ = 0;
	this->Already_used_ = 0;
	this->Storage_ = new T[0];
    }
  
    void Push_back(const value_type& value){
        if (Already_used_ < Storage_size_) {
            Storage_[Already_used_] = value;
            ++Already_used_;
            return;
        }

        int next_size = 1;
        if (Storage_size_)
            next_size = Storage_size_ * 2;

        TVector next(next_size);
        next.Already_used_ = Already_used_;

        if (Storage_ )
            std::copy(Storage_, Storage_ + Storage_size_, next.Storage_);

        next.Push_back(value);
        swap(*this, next);
    }

    const value_type& At(int index) const{
        if (index < 0 || index > Already_used_)
            throw std::out_of_range("You are doing this wrong!");

        return Storage_[index];
    }

    value_type& At(int index){
        const value_type& elem = const_cast<const TVector*>(this)->At(index);
        return const_cast<value_type&>(elem);
    }

    const value_type& operator[](int index) const{
        return At(index);
    }

    value_type& operator[](int index){
        return At(index);
    } 

private:
    int Already_used_;
    int Storage_size_;
    value_type* Storage_;
};

template <typename R> TVector<R> CountingSort(const TVector<R> &vec, int x) {
    int max = 9;
    TVector<unsigned int> c{max + 1};
    for (int i = 0; i <= max; i++){
        c[i] = 0;
    }
    for (int i = 0; i < vec.Size(); ++i){ 
        ++c[vec[i].num[x]];
    }
    for (int i = 1; i <= max; i++){ 
        c[i] += c[i-1];
    }
    TVector<R> tmp_vec{vec.Size()};
    for (int i = vec.Size() - 1; i >= 0; i--){ 
        tmp_vec[--c[vec[i].num[x]]] = vec[i];
    }
    return tmp_vec;
}
template <typename R> TVector<R> RadixSort(TVector<R> &vec) {
    TVector<R> res = vec;
    for(int i = 7; i >=0; i--){
        vec = res;
	res = CountingSort(vec, i);
    }
    return res;
}
  
}

const int SIZE  = 11;

struct TS{
    char key[SIZE];
    unsigned long long int data;
    int num[8];
    TS();
    friend std::istream &operator>>(std::istream &in, TS &object);
    friend std::ostream &operator<<(std::ostream &out, TS &object);
};

TS::TS(){
    for(unsigned int i = 0; i < sizeof(key) / sizeof(char); i++){
        key[i] = '\0';
    }
    data = 0;
    for (unsigned int i = 0; i < 8; i ++){
        num[i] = 0;
    }
}

std::istream &operator>>(std::istream &in, TS &object) {
    in >> object.key >> object.data;
    int dd = 0, mm = 0, ee = 0;
    int k = 0;
    while (object.key[k] != '.'){  
        dd *= 10;
	dd += object.key[k] - '0';
	k++;
    }
    k++;
    while (object.key[k] != '.'){  
        mm *= 10;
	mm += object.key[k] - '0';
	k++;
    }
    k++;
    while (object.key[k] != '\0'){
        ee *= 10;
	ee += object.key[k] - '0';
	k++;
    }
    object.num[0] = ee / 1000;
    object.num[1] = (ee % 1000) / 100;
    object.num[2] = (ee % 100) / 10;
    object.num[3] = (ee % 10);
    object.num[4] = mm / 10;
    object.num[5] = mm % 10;
    object.num[6] = dd / 10;
    object.num[7] = dd % 10;
    return in;
}

std::ostream &operator<<(std::ostream &out, TS &object) {
    out << object.key << '\t' << object.data;
    return out;
}

int main(){
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    NVec::TVector<TS> vec;
    TS object;
    while(std::cin >> object){
        vec.Push_back(object);
    }
    
    NVec::TVector<TS> res = NVec::RadixSort(vec);
    
    for(int i = 0; i < vec.Size(); i++){
	std::cout << res[i] << std::endl;      
	}
    return 0;
}
