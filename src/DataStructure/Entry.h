#ifndef ENTRY_H_
#define ENTRY_H_

template <typename K, typename V>
struct Entry {
    K key;
    V value;
    Entry(K k = K(), V v = V()):key(k), value(v) {}
    Entry(const Entry<K, V>& e) : key(e.key), value(e.value) {}
    bool operator<(const Entry<K, V>& e) {
        return key < e.key;
    }
    bool operator>(const Entry<K, V>& e) {
        return key > e.key;
    }
    bool operator==(const Entry<K, V>& e) {
        return key == e.key;
    }
    bool operator!=(const Entry<K, V>& e) {
        return key != e.key;
    }
};

#endif//ENTRY_H_