//
// Created by koalaa13 on 5/17/19.
//

#include "my_vector.h"
#include <memory>
#include <cstring>

size_t my_vector::get_new_capacity(size_t const &cur_cap) {
    return cur_cap == 0 ? SMALL_SIZE : cur_cap * 2;
}

u_int *copy_data(const u_int *source, size_t const &count, size_t const &capacity) {
    u_int *res = new u_int[capacity];
    memcpy(res, source, count * sizeof(u_int));
    memset(res + count, 0, (capacity - count) * sizeof(u_int));
    return res;
}

bool have_to_decrease(size_t const &size_, size_t const &capacity_) {
    return size_ * 4 < capacity_;
}

bool my_vector::is_big() const {
    return data != union_data.small_data;
}

void my_vector::change_capacity(size_t const &new_cap) {
    if (is_big()) {
        union_data.big_data.p.reset(copy_data(data, sz, new_cap));
        union_data.big_data.capacity = new_cap;
        data = union_data.big_data.p.get();
    } else {
        if (new_cap > SMALL_SIZE) {
            new(&union_data.big_data) big(copy_data(data, sz, new_cap), new_cap);
            data = union_data.big_data.p.get();
        }
    }
}

void my_vector::reserve(size_t const &new_cap) {
    if (new_cap > get_capacity()) {
        change_capacity(new_cap);
    }
}

my_vector::my_vector() : sz(0) {
    data = union_data.small_data;
    memset(data, 0, SMALL_SIZE * sizeof(u_int));
}

my_vector::my_vector(size_t const &new_size) : my_vector() {
    reserve(new_size);
    sz = new_size;
}

my_vector::~my_vector() {
    if (is_big()) {
        union_data.big_data.~big();
    }
}

my_vector::my_vector(my_vector const &other) : sz(other.sz) {
    if (other.is_big()) {
        new(&union_data.big_data) big(other.union_data.big_data);
        data = union_data.big_data.p.get();
    } else {
        memcpy(union_data.small_data, other.union_data.small_data, SMALL_SIZE * sizeof(u_int));
        data = union_data.small_data;
    }
}

void my_vector::resize(size_t const &new_size) {
    reserve(new_size);
    sz = new_size;
}

size_t my_vector::size() const {
    return sz;
}

bool my_vector::empty() const {
    return sz == 0;
}

void my_vector::push_back(const u_int &val) {
    if (get_capacity() == sz) {
        reserve(get_new_capacity(sz));
    }
    divide();
    data[sz++] = val;
}

size_t my_vector::get_capacity() const {
    return is_big() ? union_data.big_data.capacity : SMALL_SIZE;
}

void my_vector::pop_back() {
    if (is_big() && have_to_decrease(sz, get_capacity())) {
        size_t new_capacity = get_capacity() / 2;
        if (new_capacity <= SMALL_SIZE) {
			u_int tmp[SMALL_SIZE];
			memcpy(tmp, data, sz * sizeof(u_int));
			union_data.big_data.~big();
			memcpy(union_data.small_data, tmp, sz * sizeof(u_int));
			memset(union_data.small_data + sz, 0, (SMALL_SIZE - sz) * sizeof(u_int));
			data = union_data.small_data;
        } else {
            reserve(new_capacity);
        }
    }
    divide();
    --sz;
}

u_int &my_vector::operator[](size_t const &ind) {
    divide();
    return data[ind];
}

u_int const &my_vector::operator[](size_t const &ind) const {
    return data[ind];
}

my_vector &my_vector::operator=(my_vector const &other) {
    my_vector temp(other);
    swap(temp);
    return *this;
}

void my_vector::swap(my_vector &other) {
    using std::swap;
    if (!is_big() && !other.is_big()) {
        for (size_t i = 0; i < SMALL_SIZE; ++i) {
            swap(union_data.small_data[i], other.union_data.small_data[i]);
        }
    } else {
        if (is_big() && !other.is_big()) {
            swap_big_and_small(union_data, other.union_data);
            data = union_data.small_data;
            other.data = other.union_data.big_data.p.get();
        } else {
            if (!is_big() && other.is_big()) {
                swap_big_and_small(other.union_data, union_data);
                data = union_data.big_data.p.get();
                other.data = other.union_data.small_data;
            } else {
                swap(union_data.big_data, other.union_data.big_data);
                data = union_data.big_data.p.get();
                other.data = other.union_data.big_data.p.get();
            }
        }
    }
    swap(sz, other.sz);
}

void my_vector::swap_big_and_small(my_vector::any_data &big_, my_vector::any_data &small_) {
    u_int temp[SMALL_SIZE];
    memcpy(temp, small_.small_data, SMALL_SIZE * sizeof(u_int));
    new(&small_.big_data) big(big_.big_data);
    big_.big_data.~big();
    memcpy(big_.small_data, temp, SMALL_SIZE * sizeof(u_int));

}

u_int my_vector::back() {
    return data[sz - 1];
}

bool operator==(my_vector const &a, my_vector const &b) {
    if (a.sz != b.sz) {
        return false;
    }
    for (size_t i = 0; i < a.sz; ++i) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}

bool operator!=(my_vector const &a, my_vector const &b) {
    return !(a == b);
}

void my_vector::divide() {
    if (is_big() && !union_data.big_data.p.unique()) {
        union_data.big_data.p.reset(copy_data(data, sz, sz));
        union_data.big_data.capacity = sz;
        data = union_data.big_data.p.get();
    }
}

my_vector::big::big(u_int *ptr, size_t const &cap) : capacity(cap), p(ptr) {}
