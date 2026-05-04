use std::sync::{Arc, Mutex};
use std::thread;

fn f(v: Arc<Mutex<Vec<i32>>>) {
    for i in 0..1000 {
        v.lock().unwrap().push(i);
    }
}

fn main() {
    let v = Arc::new(Mutex::new(Vec::new()));
    let v1 = Arc::clone(&v);
    let v2 = Arc::clone(&v);
    let t1 = thread::spawn(move || f(v1));
    let t2 = thread::spawn(move || f(v2));
    t1.join().unwrap();
    t2.join().unwrap();
    println!("{}", v.lock().unwrap().len());
}
