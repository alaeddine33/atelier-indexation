set serveroutput on;
declare
    img VARCHAR(10);

    sig1 ordsys.ordimageSignature;
    dist float;
    
    ligne multimedia%ROWTYPE;
    cursor mm is
        select nom, signature from multimedia;
    
    min_dist FLOAT;
    min_dist_nom VARCHAR(10);
    
    startTime TIMESTAMP;
    endTime TIMESTAMP;
begin
    
    img := &img;
    
    startTime := CURRENT_TIMESTAMP;

    select signature into sig1
    from multimedia
    where nom = img;
    
    min_dist := 100;
    
    for ligne in mm loop
        dist := ordsys.ordimageSignature.evaluateScore(sig1, ligne.signature, 'color = 0, texture = 0, shape = 0.5, location = 0');
        IF(dist < min_dist) AND (ligne.nom != img) THEN
            min_dist := dist;
            min_dist_nom := ligne.nom;
        END IF;
    end loop;
    
    endTime := CURRENT_TIMESTAMP;
    
    dbms_output.put_line('img=' || min_dist_nom);
    dbms_output.put_line('diff=' || to_char(min_dist/100));
    dbms_output.put_line('time=' || to_char(endTime-startTime));

end;