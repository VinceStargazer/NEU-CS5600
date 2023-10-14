import java.util.Random;

public class Generator implements Runnable {
    private static final int STRINGS_PER_THREAD = 30;
    private static final int RANDOM_NUMBER_DIGITS = 5;
    private final BoundedBuffer buffer;
    private final Random random;

    public Generator(BoundedBuffer buffer) {
        this.buffer = buffer;
        this.random = new Random();
    }

    @Override
    public void run() {
        try {
            for (int i = 0; i < STRINGS_PER_THREAD; i++) {
                String data = Thread.currentThread().getId() + "-" + generateRandomNumber();
                buffer.deposit(data);
                System.out.println("Deposited: " + data);
            }
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    private String generateRandomNumber() {
        int number = random.nextInt((int) Math.pow(10, RANDOM_NUMBER_DIGITS));
        return String.format("%0" + RANDOM_NUMBER_DIGITS + "d", number);
    }
}