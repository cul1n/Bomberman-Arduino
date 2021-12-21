#include "List.h"

template <typename T>
List<T>::List() {
  length = 0;
}

template <typename T>
T& List<T>::getItem(byte index) {
  if (index < length)
    return data[index];
  return data[length - 1];
}

template <typename T>
void List<T>::append(T item) {
    if (length < longListLength) data[length++] = item;
}

template <typename T>
void List<T>::remove(byte index) {
    if (index < length) {
      memmove(&data[index], &data[index + 1], (length - index - 1) * sizeof(*data));
      length--;
    }
}

template <typename T>
ShortList<T>::ShortList() {
  length = 0;
}

template <typename T>
T& ShortList<T>::getItem(byte index) {
  if (index < length)
    return data[index];
  return data[length - 1];
}

template <typename T>
void ShortList<T>::append(T item) {
    if (length < shortListLength) data[length++] = item;
}

template <typename T>
void ShortList<T>::remove(byte index) {
    if (index < length) {
      memmove(&data[index], &data[index + 1], (length - index - 1) * sizeof(*data));
      length--;
    }
}
