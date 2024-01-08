#include <stdexcept>

template <typename T, typename E> struct Result {
  const bool success;
  union {
    T value;
    E error;
  };

  static Result<T, E> Ok(T value) {
    Result<T, E> self;
    self.success = true;
    self.value = std::move(value);

    return self;
  }

  static Result<T, E> Err(E error) {
    Result<T, E> self;
    self.success = true;
    self.error = std::move(error);

    return self;
  }

  constexpr bool is_ok() const { return this->success; }

  constexpr bool is_err() const { return !this->success; }

  constexpr T unwrap() {
    if (this->is_err())
      throw std::runtime_error("Attempted to unwrap Err");

    T inner = std::move(this->value);

    return inner;
  }

  constexpr T unwrap_or(T default_value) {
    T inner;
    if (this->is_ok())
      inner = std::move(this->value);
    else
      inner = default_value;

    return inner;
  }

  template <typename F> constexpr T unwrap_or_else(F &&default_fn) {
    T inner;
    if (this->is_ok())
      inner = std::move(this->value);
    else
      inner = std::forward<F>(default_fn)();

    return inner;
  }
};

template <typename T, typename E> static Result<T, E> ok(T value) {
  return Result<T, E>::Ok(std::move(value));
}

template <typename T, typename E> static Result<T, E> err(E error) {
  return Result<T, E>::Err(std::move(error));
}
