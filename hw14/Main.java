public class Main {
    private static final int THREAD_COUNT = 3;

    public static void main(String[] args) {
        BoundedBuffer buffer = new BoundedBuffer(100);
        Thread[] threads = new Thread[THREAD_COUNT];

        for (int i = 0; i < THREAD_COUNT; i++) {
            threads[i] = new Thread(new Generator(buffer));
            threads[i].start();
        }
    }
}
