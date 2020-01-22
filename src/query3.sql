set serveroutput on;
declare    
    CURSOR mm_c IS
        SELECT nom, moyenne_norme_gradient FROM multimedia;
begin
    FOR mm IN mm_c LOOP
        IF(mm.moyenne_norme_gradient > 50) THEN
            DBMS_OUTPUT.PUT_LINE(mm.nom);
        END IF;
    END LOOP;
end;
