#include <stdio.h>
#include "htmlTable.h"

/**
 * Versión simple de test, no se controlan errores
  */

int
main(void) {

    htmlTable table = newTable("cTableTest.html", 3, "Column 1", "Column 2", "Column 3");

    for(int i = 0; i < 10; i++) {
        addHTMLRow(table, "Value 1", "Value 2", "Value 3");
    }
    closeHTMLTable(table);

    return 0;
}
