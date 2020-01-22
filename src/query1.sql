set serveroutput on;
declare    
    CURSOR taux_c IS
        SELECT nom, TAUX_COULEURS FROM multimedia;
begin
    FOR mm IN taux_c LOOP
        IF(mm.TAUX_COULEURS(1) > 0.4 AND mm.TAUX_COULEURS(2) < 0.3) THEN
            DBMS_OUTPUT.PUT_LINE(mm.nom);
        END IF;
    END LOOP;
end;
