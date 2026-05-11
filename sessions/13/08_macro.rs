// 08_macro.rs — why "it's not the C preprocessor" doesn't mean macros are harmless.
//
// `macro_rules!` runs at compile time: the macro body is a **template** that pastes
// fragments of syntax into your program. There is no separate function call; there
// is no guarantee that an `expr` fragment is evaluated exactly once.

/// Naïve "maximum" of two expressions. Reads like `std::cmp::max` but is **not** a
/// function: every place `$a` / `$b` appears in the expansion is another full
/// evaluation of that expression, with all side effects, allocations, locks, etc.
///
/// This is the same failure mode as `#define max(a,b) ((a)>(b)?(a):(b))` in C.
macro_rules! evil_max {
    ($a:expr, $b:expr) => {
        if $a > $b {
            $a
        } else {
            $b
        }
    };
}

/// Looks helpful: print a label, then "return" the value so you can embed it in an
/// expression. But `$e` is expanded **twice** — once for the `println!` and again as
/// the "result". Anything non-idempotent (I/O, randomness, mutation, iterator `.next()`)
/// will diverge from what a real function argument would do.
macro_rules! log_and_use {
    ($label:expr, $e:expr) => {{
        println!("{}: {:?}", $label, $e);
        $e
    }};
}

fn bump() -> i32 {
    // Static state so we can *see* duplicate evaluation in output.
    use std::sync::atomic::{AtomicI32, Ordering};
    static N: AtomicI32 = AtomicI32::new(0);
    N.fetch_add(1, Ordering::SeqCst) + 1
}

fn main() {
    // First `evil_max!(bump(), bump())`:
    // Condition evaluates left `bump()` then right `bump()` → counter 1 and 2.
    // `1 > 2` is false, so we take the `else` branch and evaluate **right** `bump()` again → 3.
    // Return value is 3, but three increments happened for "two" arguments.
    let m = evil_max!(bump(), bump());
    println!("evil_max result = {m}   (two argument sites, three increments)");

    // Second: `evil_max!(bump(), 0)`:
    // Condition: left `bump()` → 4, compare `4 > 0` → true.
    // True branch returns `$a` again → **another** `bump()` → 5. Pure literal `0` is never re-run.
    let a = evil_max!(bump(), 0);
    println!("evil_max(bump(), 0) = {a}");

    // `log_and_use!`: same metavariable `$e` appears twice in the template → two evaluations.
    println!("--- log_and_use: same expr printed and 'returned' ---");
    let x = log_and_use!("sensor", bump());
    println!("x = {x}   (println saw one value, binding got the next)");
}
