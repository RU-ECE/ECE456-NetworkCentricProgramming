fn alloc(n: i32) -> int* {
    int* p = new int[n];
    return p;
}

// in rust, we can write unsafe code and make mistakes
// but we are not supposed to, and anyone using our
// code can tell if it's unsafe
fn main() {
    let p: *mut i32 = unsafe { alloc(10) };
    println!("{:?}", p);
}