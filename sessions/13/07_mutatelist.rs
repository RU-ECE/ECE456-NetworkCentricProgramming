fn f(v: &mut Vec<i32>) {
    for i in 0..v.len() {
        if v[i] % 2 == 0 {
            v[i] = v[i] * 2;
        }
    }
}

fn main() {
    let mut v = vec![1, 2, 3, 4, 5];
    f(&mut v); // "borrow mutably" there can be only one mutable!
    println!("{:?}", v);
}