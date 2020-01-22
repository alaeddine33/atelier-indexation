create type histo_type AS VARRAY(256) OF NUMERIC;
/
create type taux_type AS VARRAY(3) OF FLOAT;
/
create table multimedia (
  nom varchar2(50),
  image ordsys.ordimage,
  signature ordsys.ordimageSignature,
  histogramme histo_type,
  moyenne_norme_gradient FLOAT,
  nb_pixel_countour NUMERIC,
  taux_couleurs taux_type
);