truncate table multimedia;
declare
  i ordsys.ordimage;
  ctx RAW(500) := NULL;
  ligne multimedia%ROWTYPE;
  cursor mm is
  select * from multimedia
  for update;
  currname varchar(10);
begin

  for iter in 1..500
  loop
    currname := concat(to_char(iter), '.jpg');
    -- insertion d'une image, de contenu vide
    insert into multimedia(nom, image, signature)
    values (currname, ordsys.ordimage.init(), ordsys.ordimageSignature.init());
    commit;

    -- chargement du contenu de l'image a partir du fichier
    select image into i
      from multimedia
      where nom = currname
    for update;
    i.importFrom(ctx, 'file', 'IMG', currname);
    update multimedia
    set image = i
    where nom = currname;
    commit;
  end loop;
  
  -- Signatures
  for ligne in mm loop
    ligne.signature.generateSignature(ligne.image);
    update multimedia
    set signature = ligne.signature
    where current of mm;
  end loop;
  commit;
end;