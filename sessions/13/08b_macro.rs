// 08b_macro.rs — `macro_rules!` basics (compile-time syntax templates).
//
// Pattern:  macro_rules! name { ( MATCHER ) => { TEMPLATE } ; … }
// Call site: name!( …tokens… )  — the `!` reminds you this is not a function.
//
// `greet!` vs `fn greet` for a plain string: same *runtime* I/O, different mechanism.
// - Macro: no `greet` symbol at run time — the compiler pastes `println!(...)` here.
// - Macro args are *syntax* (`expr`, `ident`, `tt`, …), not a `fn` signature; type
//   checking happens only after expansion. That is how `println!("{}", x)` can be variadic.
// - For a one-off hello, **use a function**; macros earn their keep on DSL / repetition /
//   things that must expand at the **call site** (see `log_where!` below vs `log_where_fn`).

/// One arm: match a single expression, paste it into `println!`.
macro_rules! greet {
    ($who:expr) => {
        println!("Hello, {}!", $who);
    };
}

/// Same expression twice → must parenthesize, or operator precedence bites you:
/// `square!(1 + 2)` must become `(1 + 2) * (1 + 2)`, not `1 + 2 * 1 + 2`.
macro_rules! square {
    ($e:expr) => {
        ($e) * ($e)
    };
}

/// Repetition: `$( … ),*` is "zero or more comma-separated copies of …".
/// `$( $x:expr ),*` binds a list of expressions; `$( $x ),*` in the output
/// expands that list the same way (still comma-separated).
macro_rules! vec_of {
    ($($x:expr),* $(,)?) => {
        vec![$($x),*]
    };
}

fn log_where_fn() {
    // `line!` / `file!` are fixed to *this* line in the source (inside the function body).
    println!("inside fn: {}:{}", line!(), file!());
}

macro_rules! log_where {
    () => {
        // After expansion, compiler attributes `line!` / `file!` to the **invocation** site.
        println!("inside macro: {}:{}", line!(), file!());
    };
}

fn main() {
    greet!("class");
    greet!(2026_i32); // `expr` can be almost any expression

    let n = 3;
    println!("square!(n + 1) = {}", square!(n + 1)); // (3+1)*(3+1) = 16

    let v = vec_of![10, 20, 30];
    println!("{:?}", v);

    log_where_fn();
    log_where!(); // same builtins, different line numbers — macro body pasted at call site
}
