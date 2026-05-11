unsafe fn alloc(n: usize) -> *mut i32 {
    let mut values = Vec::<i32>::with_capacity(n);
    let p = values.as_mut_ptr();
    std::mem::forget(values);
    p
}

// in rust, we can write unsafe code and make mistakes
// but we are not supposed to, and anyone using our
// code can tell if it's unsafe
fn main() {
    let p: *mut i32 = unsafe { alloc(10) };
    println!("{:?}", p);
    unsafe {
        let _ = Vec::from_raw_parts(p, 0, 10);
    }
}
