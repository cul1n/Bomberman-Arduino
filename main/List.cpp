#include "List.h"

List::List() {
  length = 0;
}

Entity List::getItem(byte index) {
  if (index < length)
    return data[index];
  Entity newEntity;
  return newEntity;
}

void List::append(Entity item) {
    if (length < 16) data[length++] = item;
}

void List::remove(byte index) {
    if (index < length) {
      memmove(&data[index], &data[index + 1], (length - index - 1) * sizeof(*data));
      length--;
    }
}
