fn early_return(n: i32) -> i32 {
    for i in 1..n {
        if i % 17 == 0 {
            return i;
        }
    }
    -1
}

fn main() {
    println!("{}", early_return(100));
}
