fn f(n: i32) -> Option<i32> {
    if n < 0 {
        None
    } else {
        Some(n * 2)
    }
}

fn main() {
    let x = f(10);
    println!("{:?}", x);
    let x = f(-1);
    println!("{:?}", x);
    let x = f(-5);
    match x {
        Some(x) => println!("{}", x),
        None => println!("yo!"), // extra comma on the end is NOT REQUIRED BUT ALLOWED
    }
}