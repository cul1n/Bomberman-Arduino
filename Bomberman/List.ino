#include "List.h"

template <typename T>
List<T>::List() {
  length = 0;
}

template <typename T>
T& List<T>::getItem(byte index) {
  if (index < length)
    return data[index];
  T newObject;
  return newObject;
}

template <typename T>
void List<T>::append(T item) {
    if (length < 16) data[length++] = item;
}

template <typename T>
void List<T>::remove(byte index) {
    if (index < length) {
      memmove(&data[index], &data[index + 1], (length - index - 1) * sizeof(*data));
      length--;
    }
}
