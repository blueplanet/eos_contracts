#include <eosiolib/eosio.hpp>
#include <string>

using std::string;

class shop : public eosio::contract {
  public:
    using contract::contract;

    /// @abi action
    void create(account_name user, string title, string description, string image_url, uint64_t price, uint64_t stock) {
      require_auth(user);

      products product_table(_self, _self);
      product_table.emplace(user, [&](auto &product_rec) {
        product_rec.id = product_table.available_primary_key();

        product_rec.title = title;
        product_rec.description = description;
        product_rec.image_url = image_url;
        product_rec.price = price;
        product_rec.stock = stock;
        product_rec.creator = user;
      });
    }

    /// @abi action
    void destroy(account_name owner, uint64_t id) {
      require_auth(owner);

      products product_table(_self, _self);
      auto product = product_table.find(id);

      eosio_assert(product != product_table.end(), "not found");
      eosio_assert(product->creator == owner, "only owner");
      product_table.erase(product);
    }

    /// @abi action
    void update(account_name owner, uint64_t id, string title, string description, string image_url, uint64_t price, uint64_t stock) {
      require_auth(owner);

      products product_table(_self, _self);
      auto product = product_table.find(id);

      eosio_assert(product != product_table.end(), "not found");
      eosio_assert(product->creator == owner, "only owner");

      product_table.modify(product, owner, [&](auto &product_rec) {
        product_rec.title = title;
        product_rec.description = description;
        product_rec.image_url = image_url;
        product_rec.price = price;
        product_rec.stock = stock;
      });
    }

  private:
    /// @abi table products
    struct product {
      uint64_t id;
      string title;
      string description;
      string image_url;
      uint64_t price;
      uint64_t stock;
      account_name creator;

      auto primary_key() const { return id; }
      account_name get_creator() const { return creator; }
    };

    typedef eosio::multi_index<N(products), product> products;
};

EOSIO_ABI(shop, (create)(destroy)(update))
