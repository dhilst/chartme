let (>>=) = (a, f) =>
  switch (a) {
  | Some(something) => f(something)
  | None => None
  };

let default = (value, default) =>
  switch (value) {
  | Some(v) => v
  | None => default
  };

let default_ = (default, value) =>
  switch (value) {
  | Some(v) => v
  | None => default
  };
