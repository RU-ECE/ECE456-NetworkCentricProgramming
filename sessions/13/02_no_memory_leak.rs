fn f(n: i32) -> Vec<i32> {
    let mut v = Vec::new();
    for i in 0..n {
        v.push(i);
    }
    v // implicit return
}

fn main() {
    let v = f(10);
    println!("{:?}", v)
}