package org.clehne.revpi.dataio;

import java.io.IOException;
import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;
import java.lang.reflect.Method;

public class DataInOutTest {

    @Retention(RetentionPolicy.RUNTIME)
    @Target({ElementType.METHOD})
    @interface Test { /* EMPTY */ }

    public static void main(String[] args) throws IOException {
	startTests();
//	miscTests();
    }
    
    private static void startTests() {
        final DataInOutTest dummy = new DataInOutTest();
        boolean succeeded = true;
        for (Method testMethod : DataInOutTest.class.getMethods()) {
            if (testMethod.getAnnotation(Test.class) != null) {
                System.out.print("Test: " + testMethod.getName());
                try {
                    testMethod.invoke(dummy);
                    System.out.println(" OK");
                } catch (final Exception e) {
                    System.out.println(" FAILED");
                    e.printStackTrace();
                    succeeded = false;
                }
            }
        }
        if (!succeeded) {
            System.out.println("unit tests went wrong".toUpperCase());
            System.exit(-1);
        }
    }
    
    @Test
    public void testDataInOutRawCreate() throws IOException {
        new DataInOut().close();
    }
    

    @Test
    public void testGetDataIn() throws IOException {
    	try (final DataInOut dio = new DataInOut()) {
    	    System.out.println("");
    	    for (int i = 1; i < 17; i++) {
        	    System.out.println("Result from getDataIn(" + i + "): " + dio.getDataIn(i) );
			}
    	}
    }

    @Test
    public void testGetDataOut() throws IOException {
    	try (final DataInOut dio = new DataInOut()) {
    	    System.out.println("");
    	    for (int i = 1; i < 17; i++) {
        	    System.out.println("Result from getDataOut(" + i + "): " + dio.getDataOut(i) );
			}
    	}
    }

    @Test
    public void testSetDataOut() throws IOException {
    	try (final DataInOut dio = new DataInOut()) {

    	    System.out.println("");
    	    for (int i = 1; i < 17; i++) {
        	    System.out.println("setDataOut(" + i + ", 1)");
        	    dio.setDataOut(i, true);
			}
    	    System.out.println("pause");
    	    try {
				Thread.sleep(2000);
			} catch (InterruptedException e) {}
    	    for (int i = 1; i < 17; i++) {
        	    System.out.println("setDataOut(" + i + ", 0)");
        	    dio.setDataOut(i, false);
			}
    	}
    }

    
    
}
