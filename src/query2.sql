set serveroutput on;
declare    
    CURSOR taux_c IS
        SELECT nom, TAUX_COULEURS FROM multimedia;
begin
    FOR mm IN taux_c LOOP
        IF(mm.taux_couleurs(1) = mm.taux_couleurs(2) AND mm.taux_couleurs(2) = mm.taux_couleurs(3)) THEN
            DBMS_OUTPUT.PUT_LINE(mm.nom);
        END IF;
    END LOOP;
end;
